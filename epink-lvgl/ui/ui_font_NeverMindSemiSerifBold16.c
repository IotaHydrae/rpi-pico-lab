/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font /home/developer/sources/epink_desktop/squareline_studio_project/assets/NeverMindSemiSerif-Bold.ttf -o /home/developer/sources/epink_desktop/squareline_studio_project/assets/ui_font_NeverMindSemiSerifBold16.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_NEVERMINDSEMISERIFBOLD16
#define UI_FONT_NEVERMINDSEMISERIFBOLD16 1
#endif

#if UI_FONT_NEVERMINDSEMISERIFBOLD16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xf,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xb0,

    /* U+0023 "#" */
    0xc, 0xc3, 0x98, 0x67, 0x3f, 0xf7, 0xfe, 0x33,
    0x6, 0x63, 0xff, 0xff, 0xc6, 0x70, 0xcc, 0x19,
    0x80,

    /* U+0024 "$" */
    0x18, 0x3c, 0x7f, 0xc2, 0xc0, 0xf0, 0x7c, 0x3e,
    0x7, 0x3, 0xc3, 0xfe, 0x7c, 0x18,

    /* U+0025 "%" */
    0x78, 0x67, 0xe2, 0x33, 0x31, 0x9b, 0xf, 0xd8,
    0x3d, 0x80, 0x1d, 0xc0, 0xdf, 0xc, 0xd8, 0xe6,
    0xc6, 0x3e, 0x70, 0xe0,

    /* U+0026 "&" */
    0x1c, 0xf, 0xe1, 0x8c, 0x31, 0x87, 0x70, 0x7c,
    0x1f, 0x7, 0x76, 0xc7, 0xd8, 0xf3, 0xff, 0x3e,
    0x70,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x36, 0x6e, 0xcc, 0xcc, 0xcc, 0xc6, 0x63,

    /* U+0029 ")" */
    0xc6, 0x67, 0x33, 0x33, 0x33, 0x36, 0x6c,

    /* U+002A "*" */
    0x33, 0xf7, 0xbf, 0x30,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18,
    0x18,

    /* U+002C "," */
    0xff,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x3, 0x81, 0x81, 0xc0, 0xc0, 0xe0, 0x60, 0x70,
    0x30, 0x38, 0x18, 0xc, 0xe, 0x6, 0x7, 0x0,

    /* U+0030 "0" */
    0x3c, 0x7e, 0x67, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xe6, 0x7e, 0x3c,

    /* U+0031 "1" */
    0xfd, 0xb6, 0xdb, 0x6d, 0xb0,

    /* U+0032 "2" */
    0x3c, 0x7e, 0xe3, 0x43, 0x3, 0x7, 0xe, 0x1c,
    0x38, 0x70, 0xff, 0xff,

    /* U+0033 "3" */
    0x7f, 0x7f, 0x7, 0xe, 0x1c, 0x3e, 0x7, 0x3,
    0x3, 0x47, 0xfe, 0x3c,

    /* U+0034 "4" */
    0x6, 0x7, 0x7, 0x87, 0xc3, 0x63, 0xb3, 0x99,
    0xff, 0xff, 0x83, 0x1, 0x80, 0xc0,

    /* U+0035 "5" */
    0x3f, 0x3f, 0x70, 0x70, 0x7c, 0x7e, 0x7, 0x3,
    0x3, 0x47, 0xfe, 0x7c,

    /* U+0036 "6" */
    0x1c, 0x1c, 0x18, 0x38, 0x30, 0x7c, 0x7e, 0xe3,
    0xc3, 0xc3, 0xfe, 0x3c,

    /* U+0037 "7" */
    0xff, 0xff, 0x7, 0x6, 0xe, 0xc, 0x1c, 0x18,
    0x38, 0x30, 0x70, 0x70,

    /* U+0038 "8" */
    0x1e, 0x3f, 0x63, 0x63, 0x77, 0x3e, 0x7f, 0xc3,
    0xc3, 0xc7, 0x7e, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0x3e,
    0xc, 0x1c, 0x18, 0x38,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0xf0, 0xf, 0xf0,

    /* U+003C "<" */
    0x3, 0xf, 0x3e, 0xf0, 0xc0, 0xf0, 0x3c, 0xf,
    0x3,

    /* U+003D "=" */
    0xff, 0xff, 0x0, 0x0, 0xff, 0xff,

    /* U+003E ">" */
    0x80, 0xe0, 0x78, 0x1f, 0x7, 0x1f, 0x7c, 0xf0,
    0x80,

    /* U+003F "?" */
    0x38, 0xfb, 0x98, 0x30, 0xe3, 0x8e, 0x18, 0x30,
    0x0, 0xc1, 0x80,

    /* U+0040 "@" */
    0xf, 0xc0, 0x7f, 0xc3, 0x87, 0x98, 0x6, 0xee,
    0xdf, 0x3f, 0x3d, 0x8c, 0xf6, 0x33, 0xd8, 0xcf,
    0x7f, 0x36, 0xff, 0x9c, 0x1c, 0x3f, 0x0, 0x3c,
    0x0,

    /* U+0041 "A" */
    0x2, 0x0, 0xc0, 0x18, 0x7, 0x80, 0xf0, 0x3f,
    0x6, 0x61, 0xce, 0x3f, 0xcf, 0xf9, 0x83, 0xf0,
    0x30,

    /* U+0042 "B" */
    0xfc, 0xfe, 0xc6, 0xc6, 0xc6, 0xfc, 0xfe, 0xc3,
    0xc3, 0xc3, 0xfe, 0xfc,

    /* U+0043 "C" */
    0x1f, 0xf, 0xf7, 0x1f, 0x82, 0xc0, 0x30, 0xc,
    0x3, 0x1, 0xe0, 0xd8, 0x77, 0xf8, 0x7c,

    /* U+0044 "D" */
    0xfe, 0x3f, 0xe6, 0x19, 0x83, 0x60, 0xd8, 0x36,
    0xd, 0x83, 0x61, 0xd8, 0x67, 0xf9, 0xf8,

    /* U+0045 "E" */
    0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xfe, 0xfe, 0xc0,
    0xc0, 0xc0, 0xff, 0xff,

    /* U+0046 "F" */
    0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xfe, 0xfe, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0047 "G" */
    0xf, 0x87, 0xf9, 0xc3, 0x30, 0xc, 0x1, 0x80,
    0x30, 0xfe, 0x1f, 0xc0, 0xec, 0x3d, 0xff, 0x8f,
    0xb0,

    /* U+0048 "H" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1f, 0xff, 0xff,
    0x83, 0xc1, 0xe0, 0xf0, 0x78, 0x30,

    /* U+0049 "I" */
    0xff, 0x66, 0x66, 0x66, 0x66, 0xff,

    /* U+004A "J" */
    0xe, 0x1c, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc,
    0x1b, 0xf7, 0xc0,

    /* U+004B "K" */
    0xc3, 0xb1, 0xcc, 0xe3, 0x38, 0xdc, 0x3e, 0xf,
    0x83, 0x70, 0xce, 0x31, 0xcc, 0x73, 0xe,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xff, 0xff,

    /* U+004D "M" */
    0x80, 0x1c, 0x3, 0xe0, 0x7f, 0xf, 0xf9, 0xfd,
    0xff, 0xcf, 0x3c, 0x63, 0xc0, 0x3c, 0x3, 0xc0,
    0x3c, 0x3,

    /* U+004E "N" */
    0x80, 0xf0, 0x3e, 0xf, 0xc3, 0xf8, 0xff, 0x3c,
    0xef, 0x1f, 0xc3, 0xf0, 0x7c, 0xf, 0x1,

    /* U+004F "O" */
    0x1f, 0x7, 0xf1, 0xc7, 0x70, 0x7c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xe0, 0xee, 0x38, 0xfe, 0xf,
    0x80,

    /* U+0050 "P" */
    0x7e, 0x3f, 0x98, 0x6c, 0x36, 0x1f, 0xfb, 0xf8,
    0xc0, 0x60, 0x30, 0x18, 0xc, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x7, 0xf1, 0xc7, 0x70, 0x7c, 0x7, 0x80,
    0xf0, 0x1e, 0xb, 0xe3, 0xee, 0x38, 0xff, 0x8f,
    0xa0,

    /* U+0052 "R" */
    0xfc, 0x7f, 0xb0, 0xd8, 0x6c, 0x37, 0xf3, 0xf1,
    0x9c, 0xc6, 0x63, 0xb0, 0xd8, 0x70,

    /* U+0053 "S" */
    0x7e, 0xff, 0xc3, 0xc0, 0xf0, 0x7c, 0x3e, 0x7,
    0x3, 0xc3, 0xfe, 0x7c,

    /* U+0054 "T" */
    0xff, 0xff, 0xc6, 0x3, 0x1, 0x80, 0xc0, 0x60,
    0x30, 0x18, 0xc, 0x6, 0x3, 0x0,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xf1, 0xdf, 0xc7, 0xc0,

    /* U+0056 "V" */
    0xe0, 0x77, 0xe, 0x70, 0xe3, 0x1c, 0x39, 0xc1,
    0x98, 0x1f, 0x81, 0xf0, 0xf, 0x0, 0xe0, 0x6,
    0x0, 0x40,

    /* U+0057 "W" */
    0xe3, 0x83, 0xb0, 0xc1, 0x9c, 0x71, 0xc6, 0x18,
    0xc3, 0x9c, 0xe0, 0xcf, 0x60, 0x7f, 0xf0, 0x1e,
    0xf8, 0xf, 0x38, 0x3, 0x1c, 0x1, 0x8c, 0x0,
    0x82, 0x0,

    /* U+0058 "X" */
    0x70, 0x71, 0xc7, 0x7, 0x38, 0x3b, 0x80, 0xf8,
    0x3, 0x80, 0x3e, 0x3, 0xb8, 0x1d, 0xc1, 0xc7,
    0x1c, 0x1c, 0xe0, 0xe0,

    /* U+0059 "Y" */
    0xe1, 0xdc, 0x77, 0x38, 0xec, 0x3f, 0x7, 0x81,
    0xe0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0x7f, 0xdf, 0xe0, 0x38, 0x1c, 0x7, 0x3, 0x81,
    0xc0, 0x70, 0x38, 0x1c, 0x7, 0xff, 0xff,

    /* U+005B "[" */
    0xff, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcf, 0xf0,

    /* U+005C "\\" */
    0xe0, 0xc1, 0x83, 0x83, 0x6, 0xe, 0xc, 0x18,
    0x38, 0x30, 0x60, 0xe0, 0xc0,

    /* U+005D "]" */
    0xff, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f, 0xf0,

    /* U+005E "^" */
    0x30, 0xe7, 0x9b, 0xcc,

    /* U+005F "_" */
    0xff, 0xf0,

    /* U+0060 "`" */
    0x46, 0x20,

    /* U+0061 "a" */
    0x3d, 0xbf, 0xf8, 0xf8, 0x3c, 0x1e, 0xf, 0x8e,
    0xff, 0x3d, 0x80,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xdc, 0xfe, 0xe7, 0xc3, 0xc3,
    0xc3, 0xc7, 0xfe, 0xfc,

    /* U+0063 "c" */
    0x3e, 0x7f, 0xe3, 0xc0, 0xc0, 0xc0, 0xe3, 0x7f,
    0x3c,

    /* U+0064 "d" */
    0x3, 0x1, 0x80, 0xc7, 0x67, 0xf7, 0x3b, 0xd,
    0x86, 0xc3, 0x73, 0x9f, 0xe7, 0x70,

    /* U+0065 "e" */
    0x3e, 0x3f, 0xb0, 0xdf, 0xff, 0xfe, 0x3, 0x84,
    0xfe, 0x3e, 0x0,

    /* U+0066 "f" */
    0x0, 0x7c, 0xf9, 0x87, 0xef, 0xcc, 0x18, 0x30,
    0x60, 0xc1, 0x83, 0x6, 0x38, 0x60,

    /* U+0067 "g" */
    0x3d, 0xbf, 0xf8, 0xf8, 0x3c, 0x1e, 0xf, 0x8e,
    0xff, 0x3d, 0x81, 0xdf, 0xcf, 0xc0,

    /* U+0068 "h" */
    0xc0, 0x60, 0x30, 0x1b, 0xcf, 0xf7, 0x1b, 0xd,
    0x86, 0xc3, 0x61, 0xb0, 0xf8, 0x70,

    /* U+0069 "i" */
    0x66, 0xe, 0xe6, 0x66, 0x66, 0x77,

    /* U+006A "j" */
    0x33, 0x7, 0x73, 0x33, 0x33, 0x33, 0x3f, 0xe0,

    /* U+006B "k" */
    0xe0, 0x70, 0x18, 0xc, 0x76, 0x73, 0x71, 0xf0,
    0xf8, 0x6e, 0x33, 0x19, 0xcc, 0x70,

    /* U+006C "l" */
    0xee, 0x66, 0x66, 0x66, 0x66, 0x77,

    /* U+006D "m" */
    0xdc, 0xf3, 0xff, 0xce, 0x71, 0xb1, 0x86, 0xc6,
    0x1b, 0x18, 0x6c, 0x61, 0xb1, 0x87, 0xc6, 0x1c,

    /* U+006E "n" */
    0xde, 0x7f, 0xb8, 0xd8, 0x6c, 0x36, 0x1b, 0xd,
    0x87, 0xc3, 0x80,

    /* U+006F "o" */
    0x3e, 0x3f, 0xb8, 0xf8, 0x3c, 0x1e, 0xf, 0x8e,
    0xfe, 0x3e, 0x0,

    /* U+0070 "p" */
    0xef, 0x3f, 0xe7, 0x1d, 0x83, 0x60, 0xd8, 0x36,
    0x1d, 0xfe, 0x7f, 0x18, 0x6, 0x1, 0x80,

    /* U+0071 "q" */
    0x3d, 0x9f, 0xee, 0x3b, 0x6, 0xc1, 0xb0, 0x6e,
    0x39, 0xfe, 0x3d, 0x80, 0x60, 0x1c, 0x7,

    /* U+0072 "r" */
    0xef, 0xf7, 0x18, 0x61, 0x86, 0x18, 0x60,

    /* U+0073 "s" */
    0x3c, 0xff, 0x8b, 0x87, 0xc1, 0xf1, 0xff, 0x7c,

    /* U+0074 "t" */
    0x10, 0xc3, 0x3f, 0xfc, 0xc3, 0xc, 0x30, 0xc3,
    0xc7,

    /* U+0075 "u" */
    0xc3, 0x61, 0xb0, 0xd8, 0x6c, 0x36, 0x1b, 0x1d,
    0xff, 0x7b, 0x80,

    /* U+0076 "v" */
    0xe3, 0xb1, 0xd8, 0xce, 0xe3, 0x61, 0xe0, 0x70,
    0x30, 0x8, 0x0, 0x0,

    /* U+0077 "w" */
    0xe7, 0xd, 0x8c, 0x77, 0x39, 0x8c, 0xee, 0x3f,
    0xf0, 0x7b, 0xc1, 0xce, 0x3, 0x18, 0x8, 0x40,

    /* U+0078 "x" */
    0x71, 0x9e, 0xe1, 0xf0, 0x78, 0x1e, 0x7, 0xc3,
    0xb1, 0xce, 0x61, 0xc0,

    /* U+0079 "y" */
    0xe1, 0xf1, 0xd8, 0xee, 0x63, 0x71, 0xb0, 0xf8,
    0x3c, 0x1c, 0xe, 0x1e, 0xf, 0x0,

    /* U+007A "z" */
    0xff, 0xfe, 0xc, 0x1c, 0x18, 0x30, 0x70, 0x7f,
    0xff,

    /* U+007B "{" */
    0x1c, 0xf3, 0xc, 0x30, 0xcf, 0x38, 0x30, 0xc3,
    0xc, 0x30, 0xf0, 0xc0,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+007D "}" */
    0xe3, 0xc3, 0xc, 0x30, 0xc3, 0xc7, 0x30, 0xc3,
    0xc, 0x33, 0xcc, 0x0,

    /* U+007E "~" */
    0xf0, 0xff, 0x8f
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 75, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 82, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 110, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 7, .adv_w = 186, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 162, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 38, .adv_w = 235, .box_w = 13, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 187, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 56, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 76, .adv_w = 107, .box_w = 4, .box_h = 14, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 83, .adv_w = 107, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 90, .adv_w = 118, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 94, .adv_w = 155, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 103, .adv_w = 79, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 104, .adv_w = 113, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 106, .adv_w = 81, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 140, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 123, .adv_w = 153, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 104, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 147, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 147, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 177, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 155, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 155, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 150, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 147, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 155, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 81, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 80, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 243, .adv_w = 146, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 252, .adv_w = 151, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 258, .adv_w = 146, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 237, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 303, .adv_w = 184, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 186, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 347, .adv_w = 184, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 148, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 145, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 386, .adv_w = 189, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 176, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 89, .box_w = 4, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 124, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 178, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 449, .adv_w = 145, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 227, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 191, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 494, .adv_w = 200, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 165, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 206, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 542, .adv_w = 175, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 556, .adv_w = 162, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 159, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 179, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 194, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 614, .adv_w = 272, .box_w = 17, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 208, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 660, .adv_w = 180, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 675, .adv_w = 179, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 690, .adv_w = 101, .box_w = 4, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 698, .adv_w = 117, .box_w = 7, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 711, .adv_w = 97, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 719, .adv_w = 130, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 723, .adv_w = 91, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 725, .adv_w = 150, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 10},
    {.bitmap_index = 727, .adv_w = 163, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 738, .adv_w = 159, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 750, .adv_w = 148, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 759, .adv_w = 164, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 773, .adv_w = 151, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 784, .adv_w = 116, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 798, .adv_w = 158, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 812, .adv_w = 152, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 826, .adv_w = 73, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 832, .adv_w = 74, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 840, .adv_w = 152, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 854, .adv_w = 71, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 860, .adv_w = 232, .box_w = 14, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 876, .adv_w = 152, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 887, .adv_w = 163, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 898, .adv_w = 162, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 913, .adv_w = 167, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 928, .adv_w = 111, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 935, .adv_w = 124, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 943, .adv_w = 103, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 952, .adv_w = 153, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 963, .adv_w = 150, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 975, .adv_w = 231, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 991, .adv_w = 165, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1003, .adv_w = 155, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1017, .adv_w = 149, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1026, .adv_w = 112, .box_w = 6, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1038, .adv_w = 88, .box_w = 2, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 1042, .adv_w = 112, .box_w = 6, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1054, .adv_w = 154, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 1, 2,
    0, 0, 0, 3, 0, 4, 5, 4,
    6, 7, 0, 8, 9, 10, 0, 11,
    12, 13, 14, 0, 0, 0, 0, 0,
    0, 0, 15, 16, 17, 18, 0, 19,
    0, 0, 0, 20, 21, 22, 0, 0,
    7, 23, 7, 24, 25, 26, 20, 27,
    28, 29, 12, 30, 0, 31, 0, 0,
    0, 0, 32, 1, 1, 33, 1, 34,
    32, 35, 36, 0, 37, 38, 32, 35,
    1, 1, 33, 39, 40, 0, 32, 41,
    41, 42, 41, 43, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 1, 0, 2, 3, 4,
    0, 0, 5, 6, 0, 7, 8, 7,
    9, 10, 0, 11, 12, 13, 14, 13,
    15, 2, 16, 17, 18, 0, 0, 0,
    0, 0, 9, 0, 10, 0, 0, 0,
    10, 0, 0, 19, 0, 0, 20, 0,
    10, 0, 10, 0, 21, 22, 23, 24,
    24, 25, 26, 27, 0, 28, 29, 0,
    0, 0, 30, 3, 31, 31, 31, 32,
    31, 3, 3, 33, 3, 34, 35, 35,
    31, 35, 31, 35, 36, 37, 35, 38,
    38, 39, 38, 39, 0, 0, 40, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, -2, 0, 0, 0, 0, 0,
    -5, 5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -12, 0, -22,
    -12, -30, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, -7, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -25, 0, -35,
    0, -25, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -22, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -15, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -27,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -22, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -15, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -22, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -10, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, 0, -12,
    -15, -12, -10, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -12, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -20, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 0, 0, 0, -35, -20,
    -42, -12, -5, 0, -25, 0, 0, -10,
    -15, -7, 0, 0, -10, 0, -7, 0,
    0, 0, 0, 0, 0, -27, -30, -15,
    0, 0, -13, -17, -10, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -35, 0,
    -35, 0, 0, -15, -12, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, 0, -10,
    0, -10, 0, 0, 0, 0, 0, -22,
    0, 0, 0, 0, 0, -25, -10, -42,
    0, -37, 0, 0, 0, -5, -7, -7,
    -4, -7, 0, 0, 0, -22, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, 0, -15,
    -10, -22, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -10,
    -7, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, -12, 0,
    -10, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, 0, -15,
    -15, -17, -5, 0, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -25, 0,
    -30, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -25, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, -7, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -25,
    0, -22, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -15, 0, -10, 0,
    0, 0, 0, 0, 0, -12, -12, -10,
    0, 0, 0, 0, -10, -10, 0, 0,
    -12, 0, 0, 0, 0, -15, 0, 0,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -30, -7, -35,
    0, -40, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, -10, -25, 0, 0,
    0, 0, 0, 0, 0, 0, -30, 0,
    -40, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -30, 0, -7, 0, 0, -7,
    -20, -10, 0, 0, 0, -7, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, -10, -7, -15,
    0, -20, 0, 0, 0, -7, -12, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, -10, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -5, -10, 0, 0, -30, -25,
    -27, -7, 0, 0, 0, 0, 0, 0,
    -15, 0, -25, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -20, -30, -15,
    -5, 0, -15, 0, 0, -7, -7, 0,
    0, 0, 0, -10, 0, 0, -32, -15,
    -50, -12, 0, 0, -25, 0, 0, -5,
    0, 0, -40, 0, -10, 0, -7, 0,
    0, 0, 0, 0, 0, -30, -25, 0,
    -4, 0, -12, -17, -10, -10, -10, 0,
    0, 0, -2, -15, 0, 0, -40, -15,
    -42, -15, 0, 0, -15, 0, 0, 0,
    -15, 0, -40, 0, -5, 0, -7, 0,
    0, 0, 0, 0, 0, -25, -25, -10,
    0, 0, -10, -15, -5, 0, -10, 0,
    0, 0, 0, 0, 0, 0, 0, -15,
    0, -15, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, -5, 0,
    0, 0, 0, 0, 0, 0, -15, -10,
    0, 0, 0, 0, -15, -15, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, -2, -10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -25, -10, -30,
    0, -35, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, 0, -15,
    0, -22, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, -12, 0,
    -20, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -7, 0,
    -5, 0, -5, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, 0, -22,
    0, 0, 0, -15, 0, 0, -2, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, -10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 5, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -10, 0,
    0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -27, 0,
    -32, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 0, 0, 0, 0,
    -15, -10, 0, 0, 0, -10, -10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -12,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 0, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -10, 0, 0, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 43,
    .right_class_cnt     = 40,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ui_font_NeverMindSemiSerifBold16 = {
#else
lv_font_t ui_font_NeverMindSemiSerifBold16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if UI_FONT_NEVERMINDSEMISERIFBOLD16*/

