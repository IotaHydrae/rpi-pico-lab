// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "pico/bootrom.h"
#include "pico/platform.h"
#include "hardware/clocks.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

#include "backlight.h"
#include "tools.h"

#define LV_PRId32 PRId32

LV_FONT_DECLARE(fsex_16);
LV_FONT_DECLARE(fsex_20);

static lv_obj_t *scr_home;
static lv_obj_t *scr_backlight;
static lv_obj_t *scr_misc;

static lv_obj_t * btnm1_home;

static const lv_font_t * font_normal = &fsex_16;
static const lv_font_t * font_large = &fsex_20;

static bool test_bl_passed = false;
static bool test_misc_passed = false;

static void btn_home_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) {
        lv_scr_load(scr_home);
    }
}

static void btn_stress_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) {
        lv_demo_stress();
    }
}

static void btn_passed_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * obj = lv_event_get_target(e);
    bool *passed = lv_obj_get_user_data(obj);
    lv_obj_t *lbl = lv_obj_get_child(obj, 0);

    if(code == LV_EVENT_PRESSED) {

        *passed = !*passed;
        if(*passed) {
            lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), 0);
        } else {
            lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
        }
        lv_label_set_text_fmt(lbl, "Passed: %s", *passed ? "Yes" : "No");
    }
}

static void return_to_bootsel_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) {
        /* reset to bootsel mode */
        reset_usb_boot(0, 0);
    }
}

static void slider_backlight_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t *lbl = lv_obj_get_user_data(obj);

    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        lv_coord_t * s = lv_event_get_param(e);
        *s = LV_MAX(*s, 60);
    } else if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);
        if(dsc->part == LV_PART_KNOB && lv_obj_has_state(obj, LV_STATE_PRESSED)) {
            int value = lv_slider_get_value(obj);
            char buf[8];
            lv_snprintf(buf, sizeof(buf), "%"LV_PRId32, value);

            lv_point_t text_size;
            lv_txt_get_size(&text_size, buf, font_normal, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

            lv_area_t txt_area;
            txt_area.x1 = dsc->draw_area->x1 + lv_area_get_width(dsc->draw_area) / 2 - text_size.x / 2;
            txt_area.x2 = txt_area.x1 + text_size.x;
            txt_area.y2 = dsc->draw_area->y1 - 10;
            txt_area.y1 = txt_area.y2 - text_size.y;

            lv_area_t bg_area;
            bg_area.x1 = txt_area.x1 - LV_DPX(8);
            bg_area.x2 = txt_area.x2 + LV_DPX(8);
            bg_area.y1 = txt_area.y1 - LV_DPX(8);
            bg_area.y2 = txt_area.y2 + LV_DPX(8);

            lv_draw_rect_dsc_t rect_dsc;
            lv_draw_rect_dsc_init(&rect_dsc);
            rect_dsc.bg_color = lv_palette_darken(LV_PALETTE_GREY, 3);
            rect_dsc.radius = LV_DPX(5);
            lv_draw_rect(dsc->draw_ctx, &rect_dsc, &bg_area);

            lv_draw_label_dsc_t label_dsc;
            lv_draw_label_dsc_init(&label_dsc);
            label_dsc.color = lv_color_white();
            label_dsc.font = font_normal;
            lv_draw_label(dsc->draw_ctx, &label_dsc, &txt_area, buf, NULL);

            backlight_set_level(value);
            lv_label_set_text_fmt(lbl, "Backlight level : %d", value);
        }
    }
}

static int scr_backlight_init(void)
{
    scr_backlight = lv_obj_create(NULL);

    lv_obj_t *title_label = lv_label_create(scr_backlight);
    lv_label_set_text(title_label, "Backlight Test");
    lv_obj_set_style_text_font(title_label, font_large, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    /* backlight controller slider */
    lv_obj_t * slider1 = lv_slider_create(scr_backlight);
    lv_slider_set_range(slider1, 0, 100);
    lv_slider_set_value(slider1, 100, LV_ANIM_ON);
    lv_obj_align(slider1, LV_ALIGN_CENTER, 0, 20);
    lv_obj_add_event_cb(slider1, slider_backlight_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t *label_slider_val = lv_label_create(scr_backlight);
    lv_label_set_text_fmt(label_slider_val, "Backlight level : %d", lv_slider_get_value(slider1));
    lv_obj_set_style_text_font(label_slider_val, font_large, 0);
    lv_obj_align(label_slider_val, LV_ALIGN_CENTER, 0, -40);

    lv_obj_set_user_data(slider1, label_slider_val);

    /* a passed-if btn create here */
    lv_obj_t *btn_passed = create_passed_if_btn(scr_backlight, btn_passed_event_cb, &test_bl_passed);
    lv_obj_align(btn_passed, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* a fel return btn create here */
    lv_obj_t *btn_fel = create_btn(scr_backlight, "Return to FEL", return_to_bootsel_event_cb);
    lv_obj_set_style_text_font(btn_fel, font_normal, 0);
    lv_obj_set_style_bg_color(btn_fel, lv_color_black(), 0);
    lv_obj_align(btn_fel, LV_ALIGN_TOP_LEFT, 10, 10);

    /* create return-to-home button */
    lv_obj_t *btn_home = create_btn(scr_backlight, "Home", btn_home_event_cb);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);

    return 0;
}

static int scr_misc_init(void)
{
    scr_misc = lv_obj_create(NULL);

    lv_obj_t *title_label = lv_label_create(scr_misc);
    lv_label_set_text(title_label, "Misc Test");
    lv_obj_set_style_text_font(title_label, font_large, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    /* a passed-if btn create here */
    lv_obj_t *btn_passed = create_passed_if_btn(scr_misc, btn_passed_event_cb, &test_misc_passed);
    lv_obj_align(btn_passed, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* a fel return btn create here */
    lv_obj_t *btn_fel = create_btn(scr_misc, "Return to FEL", return_to_bootsel_event_cb);
    lv_obj_set_style_text_font(btn_fel, font_normal, 0);
    lv_obj_set_style_bg_color(btn_fel, lv_color_black(), 0);
    lv_obj_align(btn_fel, LV_ALIGN_TOP_LEFT, 10, 10);

    /* create return-to-home button */
    lv_obj_t *btn_home = create_btn(scr_misc, "Home", btn_home_event_cb);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);

    /* create return-to-home button */
    lv_obj_t *btn_stress = create_btn(scr_misc, "Stress Test", btn_stress_event_cb);
    lv_obj_align(btn_stress, LV_ALIGN_BOTTOM_MID, 120, -10);

    lv_obj_t *label_core_num = create_label(scr_misc, "Core Number: %d", get_core_num());
    lv_obj_align(label_core_num, LV_ALIGN_TOP_LEFT, 10, 60);

    lv_obj_t *label_cpu_speed = create_label(scr_misc, "CPU clock: %d MHz", clock_get_hz(clk_sys) / 1000000);
    lv_obj_align_to(label_cpu_speed, label_core_num, LV_ALIGN_TOP_LEFT, 0, 20);

    lv_obj_t *label_peri_speed = create_label(scr_misc, "Periph clock: %d MHz", clock_get_hz(clk_peri) / 1000000);
    lv_obj_align_to(label_peri_speed, label_cpu_speed, LV_ALIGN_TOP_LEFT, 0, 20);

    lv_obj_t *label_flash_speed = create_label(scr_misc, "Flash clock: %d MHz", FLASH_CLK_KHZ / 1000);
    lv_obj_align_to(label_flash_speed, label_peri_speed, LV_ALIGN_TOP_LEFT, 0, 20);

    lv_obj_t *label_chip_version = create_label(scr_misc, "Chip Version: 0x%02x", rp2040_chip_version());
    lv_obj_align_to(label_chip_version, label_flash_speed, LV_ALIGN_TOP_LEFT, 0, 20);

    lv_obj_t *label_rom_version = create_label(scr_misc, "Rom Version: 0x%02x", rp2040_rom_version());
    lv_obj_align_to(label_rom_version, label_chip_version, LV_ALIGN_TOP_LEFT, 0, 20);

    return 0;
}

static void btnmatrix_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_DRAW_PART_BEGIN) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
        /*When the button matrix draws the buttons...*/
        if(dsc->class_p == &lv_btnmatrix_class && dsc->type == LV_BTNMATRIX_DRAW_PART_BTN) {
            if (dsc->id == 0 && test_bl_passed)  {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_GREEN);
            } else if (dsc->id == 1 && test_misc_passed) {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_GREEN);
            }

            if (dsc->id == 0 && !test_bl_passed)  {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);
            } else if (dsc->id == 1 && !test_misc_passed) {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);
            }

        }
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);
        if (strcmp(txt, "Backlight") == 0) {
            lv_scr_load(scr_backlight);
        } else if (strcmp(txt, "Misc") == 0) {
            LV_LOG_USER("misc was pressed\n");
            lv_scr_load(scr_misc);
        }
    }
}

static const char * btnm_map[] = {
    "Backlight", "Misc", "",
};

int factory_test(void)
{
    /* initialize screens here */
    scr_home = lv_obj_create(NULL);
    lv_scr_load(scr_home);

    scr_backlight_init();
    scr_misc_init();

    lv_obj_t *label = lv_label_create(scr_home);
    lv_label_set_text(label, "Factory Test Suit");
    lv_obj_set_style_text_font(label, font_large, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

    char serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(serial, sizeof(serial)/sizeof(serial[0]));

    lv_obj_t *label_serial = lv_label_create(scr_home);
    lv_label_set_text_fmt(label_serial, "unique_board_id : 0x%s", serial);
    lv_obj_set_style_text_font(label_serial, font_large, 0);
    lv_obj_align_to(label_serial, label, LV_ALIGN_TOP_MID, 0, 25);
    lv_obj_set_style_text_color(label_serial, lv_palette_main(LV_PALETTE_RED), 0);

    btnm1_home = lv_btnmatrix_create(scr_home);
    lv_btnmatrix_set_map(btnm1_home, btnm_map);
    lv_obj_set_size(btnm1_home, 460, 200);
    lv_obj_set_style_text_font(btnm1_home, font_large, 0);
    lv_obj_add_event_cb(btnm1_home, btnmatrix_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_center(btnm1_home);

    lv_obj_t *btn_home = create_btn(scr_home, "Home", btn_home_event_cb);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);

    return 0;
}