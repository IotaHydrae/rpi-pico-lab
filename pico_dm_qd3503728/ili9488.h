#ifndef __ILI9488_H
#define __ILI9488_H

#include <stdint.h>

#define ILI9488_X_RES 480
#define ILI9488_Y_RES 320

extern int ili9488_driver_init();
extern int ili9488_video_flush(int xs, int ys, int xe, int ye, void *vmem16, uint32_t len);

#endif