#ifndef __FT6236_H
#define __FT6236_H

#include <stdint.h>
#include <stdbool.h>

#define TP_PRES_DOWN 0x80
#define TP_COORD_UD  0x40
#define CT_MAX_TOUCH  5

#define FT_REG_DEVICE_MODE 	    0x00    // Device mode, 0x00: Normal mode, 0x04: Test mode, 0x03: Factory mode
#define FT_REG_GEST_ID 			0x01    // Gesture ID
#define FT_REG_TD_STATUS 		0x02    // Touch point status

#define FT_REG_TOUCH1_XH 		0x03    // Touch 1 X high 8-bit
#define FT_REG_TOUCH1_XL 		0x04    // Touch 1 X low 8-bit
#define FT_REG_TOUCH1_YH 		0x05    // Touch 1 Y high 8-bit
#define FT_REG_TOUCH1_YL 		0x06    // Touch 1 Y low 8-bit

#define FT_REG_TH_GROUP			0x80
#define FT_REG_PERIODACTIVE	    0x88

#define	FT_REG_LIB_VER_H		0xA1
#define	FT_REG_LIB_VER_L		0xA2
#define FT_REG_CHIPER           0xA3
#define FT_REG_G_MODE 			0xA4
#define FT_REG_FOCALTECH_ID     0xA8
#define FT_REG_RELEASE_CODE_ID  0xAF
#define FT_REG_STATE            0xBC

extern int ft6236_driver_init(void);
extern bool ft6236_is_pressed(void);
extern uint16_t ft6236_read_x(void);
extern uint16_t ft6236_read_y(void);

#endif