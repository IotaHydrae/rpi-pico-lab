#include <stdio.h>

// Pico
#include "pico/stdlib.h"

// For memcpy
#include <string.h>

// Include descriptor struct definitions
// #include "usb_common.h"
// USB register definitions from pico-sdk
#include "hardware/regs/usb.h"
// USB hardware struct definitions from pico-sdk
#include "hardware/structs/usb.h"
// For interrupt enable and numbers
#include "hardware/irq.h"
// For resetting the USB controller
#include "hardware/resets.h"

#include "usb_common.h"

// #define printf(...)

#define EP0_IN_ADDR  (USB_DIR_IN  | 0)
#define EP0_OUT_ADDR (USB_DIR_OUT | 0)
#define EP1_OUT_ADDR (USB_DIR_OUT | 1)
#define EP2_IN_ADDR  (USB_DIR_IN  | 2)
#define usb_hw_set ((usb_hw_t *)hw_set_alias_untyped(usb_hw))
#define usb_hw_clear ((usb_hw_t *)hw_clear_alias_untyped(usb_hw))

typedef void (*usb_ep_handler)(uint8_t *buf, uint16_t len);

// Struct in which we keep the endpoint configuration
struct usb_endpoint_configuration {
    const struct usb_endpoint_descriptor *descriptor;
    usb_ep_handler handler;

    // Pointers to endpoint + buffer control registers
    // in the USB controller DPSRAM
    volatile uint32_t *endpoint_control;
    volatile uint32_t *buffer_control;
    volatile uint8_t *data_buffer;

    // Toggle after each packet (unless replying to a SETUP)
    uint8_t next_pid;
};

struct usb_device_configuration {
    const struct usb_device_descriptor *device_descriptor;
    const struct usb_interface_descriptor *interface_descriptor;
    const struct usb_configuration_descriptor *config_descriptor;
    const unsigned char *lang_descriptor;
    const unsigned char **descriptor_strings;
    // USB num endporint is 16
    struct usb_endpoint_configuration endpoints[USB_NUM_ENDPOINTS];
};

// Function prototypes for our device specific endpoint handlers defined
// later on
void ep0_in_handler(uint8_t *buf, uint16_t len);
void ep0_out_handler(uint8_t *buf, uint16_t len);
void ep1_out_handler(uint8_t *buf, uint16_t len);
void ep2_in_handler(uint8_t *buf, uint16_t len);

/* ------------------------ Global variables ----------------------------- */
static bool should_set_address = false;
static uint8_t dev_addr = 0;
static volatile bool configured = false;

// Global data buffer for EP0
static uint8_t ep0_buf[64];

static const struct usb_device_descriptor device_descriptor = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DT_DEVICE,
    .bcdUSB             = 0x0110,   // USB 1.1 device
    .bDeviceClass       = 0,        // Specified in interface descriptor
    .bDeviceSubClass    = 0,        // No subclass
    .bDeviceProtocol    = 0,        // No protocol
    .bMaxPacketSize0    = 64,        // Max packet size for endpoint 0
    .idVendor           = 0x2e8a,
    .idProduct          = 0x0049,
    .bcdDevice          = 0,        // No device revision number
    .iManufacturer      = 1,        // Index of string descriptor describing manufacturer
    .iProduct           = 2,        // Index of string descriptor describing product
    .iSerialNumber      = 0,        // Index of string descriptor describing the device’s serial number
    .bNumConfigurations = 1,     // one configuration
};

static const struct usb_interface_descriptor interface_descriptor = {
    .bLength            = sizeof(struct usb_interface_descriptor),
    .bDescriptorType    = USB_DT_INTERFACE,

    /* Number of this interface. Zero-based value identifying
     * the index in the array of concurrent interfaces
     * supported by this configuration. */
    .bInterfaceNumber   = 0,

    .bAlternateSetting  = 0, // Value used to select this alternate setting for the interface identified in the prior field
    .bNumEndpoints      = 2, // This interface has 2 endpoints
    .bInterfaceClass    = 0xff, // Vendor specific class
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface         = 0,
};

static const struct usb_endpoint_descriptor ep0_out = {
    .bLength = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = EP0_OUT_ADDR,
    .bmAttributes = USB_TRANSFER_TYPE_CONTROL,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

static const struct usb_endpoint_descriptor ep0_in = {
    .bLength = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = EP0_IN_ADDR,
    .bmAttributes = USB_TRANSFER_TYPE_CONTROL,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

static const struct usb_endpoint_descriptor ep1_out = {
    .bLength = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = EP1_OUT_ADDR,    // EP number 1, OUT from host (rx to device)
    .bmAttributes = USB_TRANSFER_TYPE_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

static const struct usb_endpoint_descriptor ep2_in = {
    .bLength = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = EP2_IN_ADDR,    // EP number 2, IN to host (tx from device)
    .bmAttributes = USB_TRANSFER_TYPE_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

static const struct usb_configuration_descriptor config_descriptor = {
    .bLength = sizeof(struct usb_configuration_descriptor),
    .bDescriptorType = USB_DT_CONFIG,
    .wTotalLength = (sizeof(config_descriptor) +
                     sizeof(interface_descriptor) +
                     sizeof(ep1_out) +
                     sizeof(ep2_in)),
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0xc0, // Self powered, no remote wakeup
    .bMaxPower = 0x32, // 100mA
};

static const unsigned char lang_descriptor[] = {
    4,              // bLength
    0x03,       // bDescriptorType = String Descriptor
    0x09, 0x04, // Language ID = US English
};

static const unsigned char *descriptor_strings[] = {
    (unsigned char *)"Raspberry Pi",
    (unsigned char *)"USB Display Device",
};

static struct usb_device_configuration dev_config = {
    .device_descriptor    = &device_descriptor,
    .interface_descriptor = &interface_descriptor,
    .config_descriptor = &config_descriptor,
    .lang_descriptor = lang_descriptor,
    .descriptor_strings = descriptor_strings,
    .endpoints = {
        {
            .descriptor = &ep0_out,
            .handler = ep0_out_handler,
            .endpoint_control = NULL, // NA for ep0
            .buffer_control = &usb_dpram->ep_buf_ctrl[0].out,
            .data_buffer = &usb_dpram->ep0_buf_a[0],
        },
        {
            .descriptor = &ep0_in,
            .handler = ep0_in_handler,
            .endpoint_control = NULL, // NA for ep0
            .buffer_control = &usb_dpram->ep_buf_ctrl[0].in,
            .data_buffer = &usb_dpram->ep0_buf_a[0],
        },
        {
            .descriptor = &ep1_out,
            .handler = ep1_out_handler,
            .endpoint_control = &usb_dpram->ep_ctrl[0].out,
            .buffer_control = &usb_dpram->ep_buf_ctrl[1].out,
            .data_buffer = &usb_dpram->epx_data[0 * 64],
        },
        {
            .descriptor = &ep2_in,
            .handler = ep2_in_handler,
            .endpoint_control = &usb_dpram->ep_ctrl[1].in,
            .buffer_control = &usb_dpram->ep_buf_ctrl[2].in,
            .data_buffer = &usb_dpram->epx_data[1 * 64],
        },
    }
};

struct usb_endpoint_configuration *usb_get_endpoint_configuration(uint8_t addr)
{
    struct usb_endpoint_configuration *endpoints = dev_config.endpoints;
    for (int i = 0; i < USB_NUM_ENDPOINTS; i++) {
        if (endpoints[i].descriptor && endpoints[i].descriptor->bEndpointAddress == addr) {
            return &endpoints[i];
        }
    }
    return NULL;
}

static inline uint32_t usb_buffer_offset(volatile uint8_t *buf)
{
    return (uint32_t) buf ^ (uint32_t) usb_dpram;
}

void usb_setup_endpoint(const struct usb_endpoint_configuration *ep)
{
    printf("Setup endpoint 0x%x with buffer address 0x%p\n", ep->descriptor->bEndpointAddress, ep->data_buffer);

    if (!ep->endpoint_control) {
        return;
    }

    uint32_t dpram_offset = usb_buffer_offset(ep->data_buffer);
    uint32_t reg = EP_CTRL_ENABLE_BITS
                    | EP_CTRL_INTERRUPT_PER_BUFFER
                    | (ep->descriptor->bmAttributes << EP_CTRL_BUFFER_TYPE_LSB)
                    | dpram_offset;
    *ep->endpoint_control = reg;
}

void usb_setup_endpoints() {
    const struct usb_endpoint_configuration *endpoints = dev_config.endpoints;
    for (int i = 0; i < USB_NUM_ENDPOINTS; i++) {
        if (endpoints[i].descriptor && endpoints[i].handler) {
            usb_setup_endpoint(&endpoints[i]);
        }
    }
}

void usb_device_init()
{
    reset_unreset_block_num_wait_blocking(RESETS_RESET_USBCTRL_BITS);

    memset(usb_dpram, 0, sizeof(*usb_dpram));

    irq_set_enabled(USBCTRL_IRQ, true);

    usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;

    usb_hw->pwr = USB_USB_PWR_VBUS_DETECT_BITS | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS;

    usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;

    usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS;

    usb_hw->inte = USB_INTS_BUFF_STATUS_BITS |
                  USB_INTS_BUS_RESET_BITS |
                  USB_INTS_SETUP_REQ_BITS;

    usb_setup_endpoints();

    usb_hw_set->sie_ctrl = USB_SIE_CTRL_PULLUP_EN_BITS;
}

static inline bool ep_is_tx(struct usb_endpoint_configuration *ep)
{
    return ep->descriptor->bEndpointAddress & USB_DIR_IN;
}

void usb_start_transfer(struct usb_endpoint_configuration *ep, uint8_t *buf, uint16_t len)
{
    assert(len <= 64);

    printf("%s, len %d on ep addr 0x%0x\n", __func__, len, ep->descriptor->bEndpointAddress);

    uint32_t val = len | USB_BUF_CTRL_AVAIL;

    if (ep_is_tx(ep)) {
        // need to copy the data from the user buffer to the usb memory
        memcpy((void *) ep->data_buffer, (void *) buf, len);
        // Mark as full
        val |= USB_BUF_CTRL_FULL;
    }

    // set pid and flip for next transfer
    val |= ep->next_pid ? USB_BUF_CTRL_DATA1_PID : USB_BUF_CTRL_DATA0_PID;
    ep->next_pid ^= 1u;

    *ep->buffer_control = val;
}

int main()
{
    stdio_uart_init_full(uart0, 115200, 16, 17);
    printf("\n\n\n\nUSB Device Low-Level hardware example\n");

    usb_device_init();

    while(!configured) {
        tight_loop_contents();
    }

    usb_start_transfer(usb_get_endpoint_configuration(EP1_OUT_ADDR), NULL, 64);

    for (;;) {
        tight_loop_contents();
    }
    return 0;
}

void ep0_in_handler(uint8_t *buf, uint16_t len) {
    printf("%s\n", __func__);
    if (should_set_address) {
        usb_hw->dev_addr_ctrl = dev_addr;
        should_set_address = false;
    } else {
        struct usb_endpoint_configuration *ep = usb_get_endpoint_configuration(EP0_OUT_ADDR);
        usb_start_transfer(ep, NULL, 0);
    }
}

void ep0_out_handler(__unused uint8_t *buf, __unused uint16_t len) {
    printf("%s\n", __func__);
}

void ep1_out_handler(uint8_t *buf, uint16_t len) {
    printf("%s\n", __func__);
    struct usb_endpoint_configuration *ep = usb_get_endpoint_configuration(EP2_IN_ADDR);
    usb_start_transfer(ep, buf, len);
}

void ep2_in_handler(uint8_t *buf, uint16_t len) {
    printf("%s\n", __func__);
    usb_start_transfer(usb_get_endpoint_configuration(EP1_OUT_ADDR), NULL, 64);
}

void usb_handle_device_descriptor(volatile struct usb_setup_packet *pkt)
{
    const struct usb_device_descriptor *d = dev_config.device_descriptor;

    struct usb_endpoint_configuration *ep = usb_get_endpoint_configuration(EP0_IN_ADDR);
    ep->next_pid = 1;
    usb_start_transfer(ep, (uint8_t *)d, MIN(sizeof(struct usb_device_descriptor), pkt->wLength));
}

void usb_acknowledge_out_request(void)
{
    usb_start_transfer(usb_get_endpoint_configuration(EP0_IN_ADDR), NULL, 0);
}

void usb_set_device_address(volatile struct usb_setup_packet *pkt)
{
    dev_addr = (pkt->wValue & 0xff);
    printf("Set address %d\n", dev_addr);

    should_set_address = true;
    usb_acknowledge_out_request();
}

void usb_bus_reset(void)
{
    dev_addr = 0;
    should_set_address = false;
    usb_hw->dev_addr_ctrl = 0;
    configured = false;
}

static void usb_handle_ep_buff_done(struct usb_endpoint_configuration *ep)
{
    uint32_t buffer_control = *ep->buffer_control;
    uint16_t len = buffer_control & USB_BUF_CTRL_LEN_MASK;

    ep->handler((uint8_t *) ep->data_buffer, len);
}

static void usb_handle_buff_done(uint ep_num, bool in)
{
    uint8_t ep_addr = ep_num | (in ? USB_DIR_IN : 0);
    printf("%s, EP %d (in = %d) done\n", __func__, ep_num, in);
    for (uint i = 0; i < USB_NUM_ENDPOINTS; i++) {
        struct usb_endpoint_configuration *ep = &dev_config.endpoints[i];
        if (ep->descriptor && ep->handler) {
            if (ep->descriptor->bEndpointAddress == ep_addr) {
                usb_handle_ep_buff_done(ep);
                return;
            }
        }
    }
}

static void usb_handle_buff_status()
{
    uint32_t buffers = usb_hw->buf_status;
    uint32_t remaining_buffers = buffers;

    uint bit = 1u;
    for (uint i = 0; remaining_buffers && i < USB_NUM_ENDPOINTS * 2; i++) {
        if (remaining_buffers & bit) {
            usb_hw_clear->buf_status = bit;
            usb_handle_buff_done(i >> 1u, !(i & 1u));
            remaining_buffers &= ~bit;
        }
        bit <<= 1u;
    }
}

void usb_handle_setup_packet(void)
{
    volatile struct usb_setup_packet *pkt = (volatile struct usb_setup_packet *) &usb_dpram->setup_packet;
    uint8_t req_direction = pkt->bmRequestType;
    uint8_t req = pkt->bRequest;

    // printf("bmRequestType: 0x%0x, bRequest: 0x%0x\n", req_direction, req);
    usb_get_endpoint_configuration(EP0_IN_ADDR)->next_pid = 1u;

    if (req_direction == USB_DIR_OUT) {
        if (req == USB_REQUEST_SET_ADDRESS) {
            usb_set_device_address(pkt);
            printf("OUT: Set address request\n");
        } else if (req == USB_REQUEST_GET_CONFIGURATION) {
            printf("OUT: Get configuration request\n");
        } else {
            usb_acknowledge_out_request();
            printf("Other OUT request (0x%0x)\r\n", pkt->bRequest);
        }

    } else if (req_direction == USB_DIR_IN) {
        if (req == USB_REQUEST_GET_DESCRIPTOR) {
            uint16_t descriptor_type = pkt->wValue >> 8;

            switch (descriptor_type) {
            case USB_DT_DEVICE:
                printf("IN: Get device descriptor request\n");
                usb_handle_device_descriptor(pkt);
                break;
            case USB_DT_CONFIG:
                printf("IN: Get config descriptor request\n");
                break;
            case USB_DT_STRING:
                printf("IN: Get string descriptor request\n");
                break;
            default:
                printf("Unhandled Get Descriptor type: 0x%0x\r\n", descriptor_type);
                break;
            }
        }
    } else {
        printf("Other IN request (0x%0x)\r\n", pkt->bRequest);
    }
}

#ifdef __cplusplus
extern "C" {
#endif
void isr_usbctrl(void)
{
    uint32_t status = usb_hw->ints;
    uint32_t handled = 0;

    // Setup packet received
    if (status & USB_INTS_SETUP_REQ_BITS) {
        printf("%s, setup packet received\n", __func__);
        handled |= USB_INTS_SETUP_REQ_BITS;
        usb_hw_clear->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
        usb_handle_setup_packet();
    }

    // Buffer status, one or more buffers have completed
    if (status & USB_INTS_BUFF_STATUS_BITS) {
        printf("%s, one or more buffer \n", __func__);
        handled |= USB_INTS_BUFF_STATUS_BITS;
        usb_handle_buff_status();
    }

    if (status & USB_INTS_BUS_RESET_BITS) {
        // printf("%s, bus reset\n", __func__);
        handled |= USB_INTS_BUS_RESET_BITS;
        usb_hw_clear->sie_ctrl = USB_SIE_STATUS_BUS_RESET_BITS;
        usb_bus_reset();
    }

    if (status ^ handled) {
        panic("Unhandled IRQ 0x%x\n", (uint) (status ^ handled));
    }
}
#ifdef __cplusplus
}
#endif
