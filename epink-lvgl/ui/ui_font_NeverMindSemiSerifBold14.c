/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --font /home/developer/sources/epink_desktop/squareline_studio_project/assets/NeverMindSemiSerif-Bold.ttf -o /home/developer/sources/epink_desktop/squareline_studio_project/assets/ui_font_NeverMindSemiSerifBold14.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "ui.h"

#ifndef UI_FONT_NEVERMINDSEMISERIFBOLD14
#define UI_FONT_NEVERMINDSEMISERIFBOLD14 1
#endif

#if UI_FONT_NEVERMINDSEMISERIFBOLD14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0xf0,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xb0,

    /* U+0023 "#" */
    0x19, 0x86, 0x47, 0xfd, 0xff, 0x33, 0xc, 0xcf,
    0xfb, 0xfe, 0x32, 0xd, 0x80,

    /* U+0024 "$" */
    0x30, 0xf3, 0xfe, 0x2c, 0x1e, 0x1f, 0xf, 0x7,
    0x8f, 0xfb, 0xc3, 0x0,

    /* U+0025 "%" */
    0x70, 0xcf, 0x88, 0xd9, 0x8f, 0xb0, 0x72, 0x0,
    0x4e, 0xd, 0xf0, 0x9b, 0x11, 0xb3, 0xe,

    /* U+0026 "&" */
    0x3c, 0x1f, 0x86, 0x61, 0x98, 0x3c, 0x1e, 0x4c,
    0xfb, 0x1c, 0xff, 0x9e, 0x70,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x26, 0x6c, 0xcc, 0xcc, 0xcc, 0x66, 0x20,

    /* U+0029 ")" */
    0xc6, 0x63, 0x33, 0x33, 0x33, 0x66, 0x40,

    /* U+002A "*" */
    0x27, 0xdd, 0xf2, 0x0,

    /* U+002B "+" */
    0x18, 0x30, 0x67, 0xff, 0xe3, 0x6, 0xc,

    /* U+002C "," */
    0xfc,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x6, 0x6, 0xe, 0xc, 0xc, 0x18, 0x18, 0x30,
    0x30, 0x60, 0x60, 0xe0, 0xc0,

    /* U+0030 "0" */
    0x38, 0xfb, 0xbe, 0x3c, 0x78, 0xf1, 0xf7, 0x7c,
    0x70,

    /* U+0031 "1" */
    0xfd, 0xb6, 0xdb, 0x6c,

    /* U+0032 "2" */
    0x3c, 0xff, 0x18, 0x30, 0xe3, 0x8e, 0x38, 0xff,
    0xfc,

    /* U+0033 "3" */
    0x7e, 0xfc, 0x30, 0xc3, 0xc1, 0xc1, 0xa3, 0xfe,
    0xf8,

    /* U+0034 "4" */
    0xc, 0x1c, 0x3c, 0x3c, 0x6c, 0xcc, 0xff, 0xff,
    0xc, 0xc,

    /* U+0035 "5" */
    0x7e, 0xfd, 0x83, 0xe7, 0xe0, 0xc1, 0x83, 0xfc,
    0xf0,

    /* U+0036 "6" */
    0x18, 0x60, 0xc3, 0xe7, 0xf8, 0xf1, 0xe3, 0xfc,
    0xf0,

    /* U+0037 "7" */
    0xff, 0xfc, 0x18, 0x60, 0xc3, 0xe, 0x18, 0x70,
    0xc0,

    /* U+0038 "8" */
    0x3d, 0xff, 0x1e, 0x37, 0xcf, 0xf1, 0xe3, 0xfe,
    0xf8,

    /* U+0039 "9" */
    0x3d, 0xff, 0x1e, 0x3f, 0xef, 0x87, 0xc, 0x38,
    0x60,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0xf, 0xc0,

    /* U+003C "<" */
    0x6, 0x3d, 0xe7, 0xe, 0xf, 0x7, 0x83,

    /* U+003D "=" */
    0xff, 0xfc, 0x7, 0xff, 0xe0,

    /* U+003E ">" */
    0x81, 0xc1, 0xe0, 0xf0, 0xe7, 0xb8, 0x40,

    /* U+003F "?" */
    0x3d, 0xff, 0x98, 0x31, 0xc6, 0xc, 0x0, 0x30,
    0x60,

    /* U+0040 "@" */
    0x1f, 0x83, 0xfc, 0x70, 0xe6, 0xff, 0xdf, 0xbd,
    0x9b, 0xd9, 0xbd, 0xfb, 0xef, 0xf7, 0xe, 0x3e,
    0x1, 0xe0,

    /* U+0041 "A" */
    0x4, 0x1, 0x0, 0xe0, 0x38, 0x1a, 0x6, 0xc3,
    0x30, 0xfe, 0x7f, 0x98, 0x6c, 0xc,

    /* U+0042 "B" */
    0xf9, 0xfb, 0x36, 0x6f, 0xdf, 0xf1, 0xe3, 0xff,
    0xf8,

    /* U+0043 "C" */
    0x1e, 0x3f, 0xd8, 0x78, 0xc, 0x6, 0x3, 0x3,
    0xc7, 0x7f, 0x1f, 0x0,

    /* U+0044 "D" */
    0xfe, 0x7f, 0x98, 0xec, 0x36, 0x1b, 0xd, 0x86,
    0xc7, 0x7f, 0x3f, 0x0,

    /* U+0045 "E" */
    0xff, 0xff, 0x6, 0xf, 0xdf, 0xb0, 0x60, 0xff,
    0xfc,

    /* U+0046 "F" */
    0xff, 0xff, 0x6, 0xf, 0xdf, 0xb0, 0x60, 0xc1,
    0x80,

    /* U+0047 "G" */
    0x1f, 0xf, 0xe7, 0xb, 0x80, 0xc3, 0xf0, 0xfc,
    0x1f, 0x8f, 0x7f, 0xcf, 0xb0,

    /* U+0048 "H" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3,
    0xc3, 0xc3,

    /* U+0049 "I" */
    0xfd, 0xb6, 0xdb, 0xfc,

    /* U+004A "J" */
    0xe, 0x1c, 0x30, 0x60, 0xc1, 0x83, 0x6, 0x7c,
    0xf0,

    /* U+004B "K" */
    0xc7, 0x63, 0x33, 0x1b, 0x8f, 0x87, 0xc3, 0x71,
    0x9c, 0xc6, 0x61, 0x80,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0xff, 0xf8,

    /* U+004D "M" */
    0x80, 0x38, 0xf, 0x83, 0xf8, 0xff, 0xbf, 0xbe,
    0xf3, 0x9e, 0x23, 0xc0, 0x78, 0xf, 0x1, 0x80,

    /* U+004E "N" */
    0x81, 0xe0, 0xf8, 0x7e, 0x3f, 0x9e, 0xef, 0x3f,
    0x8f, 0xc3, 0xe0, 0xf0, 0x20,

    /* U+004F "O" */
    0x1f, 0x1f, 0xe6, 0x1f, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x86, 0x7f, 0x87, 0x80,

    /* U+0050 "P" */
    0x7e, 0x7f, 0x63, 0x63, 0xff, 0xfe, 0x60, 0x60,
    0x60, 0x60,

    /* U+0051 "Q" */
    0x1f, 0x1f, 0xe6, 0x1f, 0x3, 0xc0, 0xf0, 0x3c,
    0x3f, 0x8e, 0x7f, 0xc7, 0xa0,

    /* U+0052 "R" */
    0xfc, 0xfe, 0xc6, 0xc6, 0xfe, 0xf8, 0xcc, 0xcc,
    0xc6, 0xc6,

    /* U+0053 "S" */
    0x7d, 0xff, 0x16, 0x7, 0x83, 0x81, 0xc3, 0xfe,
    0xf8,

    /* U+0054 "T" */
    0xff, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7,
    0x7e, 0x3c,

    /* U+0056 "V" */
    0xe0, 0xd8, 0x77, 0x18, 0xce, 0x33, 0x7, 0xc1,
    0xe0, 0x30, 0xc, 0x2, 0x0,

    /* U+0057 "W" */
    0xe3, 0xe, 0xc6, 0x19, 0xc6, 0x71, 0x9c, 0xc3,
    0xbb, 0x83, 0xfe, 0x7, 0xbc, 0x7, 0x30, 0xc,
    0x60, 0x8, 0x80,

    /* U+0058 "X" */
    0x70, 0xe7, 0x38, 0x6e, 0xf, 0x80, 0xe0, 0x1e,
    0x6, 0xe1, 0xcc, 0x71, 0xcc, 0x1c,

    /* U+0059 "Y" */
    0xc3, 0xf1, 0x99, 0x86, 0xc3, 0xc0, 0xe0, 0x60,
    0x30, 0x18, 0xc, 0x0,

    /* U+005A "Z" */
    0x7f, 0xbf, 0x81, 0xc0, 0xc0, 0xc0, 0xe0, 0xe0,
    0x60, 0x7f, 0xff, 0xc0,

    /* U+005B "[" */
    0xff, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xff,

    /* U+005C "\\" */
    0xc1, 0x86, 0x18, 0x30, 0xc3, 0x6, 0x18, 0x60,
    0xc3, 0xc,

    /* U+005D "]" */
    0xff, 0x33, 0x33, 0x33, 0x33, 0x33, 0xff,

    /* U+005E "^" */
    0x73, 0xb7, 0xb0,

    /* U+005F "_" */
    0xff, 0xc0,

    /* U+0060 "`" */
    0x4c,

    /* U+0061 "a" */
    0x3b, 0x7f, 0xe7, 0xc3, 0xc3, 0xc7, 0x7f, 0x3b,

    /* U+0062 "b" */
    0xc1, 0x83, 0x6, 0xef, 0xd8, 0xf1, 0xe3, 0xc7,
    0xfb, 0xf0,

    /* U+0063 "c" */
    0x3c, 0xff, 0x8e, 0xc, 0x3c, 0xdf, 0x9e,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3b, 0x7f, 0xe7, 0xc3, 0xc3,
    0xc7, 0x7f, 0x3b,

    /* U+0065 "e" */
    0x3c, 0x7e, 0xc6, 0xff, 0xff, 0xc0, 0x7e, 0x3c,

    /* U+0066 "f" */
    0x8, 0xf3, 0xcc, 0x7d, 0xf3, 0xc, 0x30, 0xc3,
    0xc, 0x33, 0xce, 0x0,

    /* U+0067 "g" */
    0x3f, 0x7f, 0xe3, 0xc3, 0xc3, 0xc7, 0x7f, 0x3b,
    0x7, 0x7e, 0x7c,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xef, 0xfc, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0069 "i" */
    0x66, 0xe, 0xe6, 0x66, 0x67, 0x70,

    /* U+006A "j" */
    0x33, 0x7, 0x73, 0x33, 0x33, 0x33, 0xfe,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0x7c, 0xdb, 0x3e, 0x7c, 0xd9,
    0x9b, 0x38,

    /* U+006C "l" */
    0xdb, 0x6d, 0xb6, 0xdf, 0x80,

    /* U+006D "m" */
    0xdd, 0xdf, 0xff, 0x99, 0xe3, 0x3c, 0x67, 0x8c,
    0xf1, 0x9e, 0x33,

    /* U+006E "n" */
    0xdc, 0xfe, 0xe6, 0xc6, 0xc6, 0xc6, 0xc7, 0xc7,

    /* U+006F "o" */
    0x3c, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0x3c,

    /* U+0070 "p" */
    0xee, 0x7f, 0x98, 0xec, 0x36, 0x1b, 0x1d, 0xfc,
    0xfc, 0x60, 0x30, 0x18, 0x0,

    /* U+0071 "q" */
    0x3b, 0x7f, 0xe7, 0xc3, 0xc3, 0xc7, 0x7f, 0x3b,
    0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xdf, 0xfe, 0x30, 0xc3, 0xc, 0x30,

    /* U+0073 "s" */
    0x7b, 0xfc, 0x7c, 0x3e, 0x3f, 0xfe,

    /* U+0074 "t" */
    0x63, 0x3f, 0xf6, 0x31, 0x8c, 0x7b, 0xc0,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x79, 0xff, 0xbb,

    /* U+0076 "v" */
    0xc3, 0x66, 0x66, 0x6c, 0x3c, 0x38, 0x18, 0x10,
    0x10,

    /* U+0077 "w" */
    0xc6, 0x33, 0x31, 0x99, 0xd8, 0x6e, 0xc3, 0xdc,
    0xe, 0xe0, 0x62, 0x1, 0x10,

    /* U+0078 "x" */
    0x73, 0x3b, 0x87, 0x83, 0x81, 0xe1, 0xb1, 0xdc,
    0xc6,

    /* U+0079 "y" */
    0xc3, 0xe3, 0x67, 0x66, 0x36, 0x3c, 0x1c, 0x1c,
    0x18, 0x78, 0x70,

    /* U+007A "z" */
    0xff, 0xf8, 0x60, 0xc3, 0xc, 0x1f, 0xff,

    /* U+007B "{" */
    0x1c, 0xf3, 0xc, 0x30, 0xce, 0x3c, 0x30, 0xc3,
    0xf, 0x1c,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xe3, 0xc3, 0xc, 0x30, 0xc3, 0xcf, 0x30, 0xc3,
    0x3c, 0xe0,

    /* U+007E "~" */
    0xf1, 0xfc, 0x78
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 66, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 72, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 96, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 7, .adv_w = 163, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 142, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 32, .adv_w = 206, .box_w = 12, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 47, .adv_w = 164, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 49, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 61, .adv_w = 94, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 68, .adv_w = 94, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 75, .adv_w = 103, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 79, .adv_w = 136, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 69, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 87, .adv_w = 98, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 89, .adv_w = 71, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 123, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 103, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 91, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 129, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 129, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 155, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 136, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 131, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 128, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 136, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 71, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 70, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 194, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 132, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 206, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 124, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 207, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 240, .adv_w = 161, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 163, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 161, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 129, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 127, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 165, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 154, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 78, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 108, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 155, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 127, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 199, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 167, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 175, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 405, .adv_w = 144, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 180, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 153, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 438, .adv_w = 142, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 139, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 157, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 169, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 480, .adv_w = 238, .box_w = 15, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 499, .adv_w = 182, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 158, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 156, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 88, .box_w = 4, .box_h = 14, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 544, .adv_w = 103, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 554, .adv_w = 85, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 561, .adv_w = 114, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 564, .adv_w = 80, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 566, .adv_w = 131, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 9},
    {.bitmap_index = 567, .adv_w = 142, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 139, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 585, .adv_w = 130, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 592, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 102, .box_w = 6, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 623, .adv_w = 138, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 634, .adv_w = 133, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 64, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 650, .adv_w = 65, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 657, .adv_w = 133, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 667, .adv_w = 62, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 672, .adv_w = 203, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 683, .adv_w = 133, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 691, .adv_w = 142, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 699, .adv_w = 142, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 712, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 723, .adv_w = 97, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 109, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 735, .adv_w = 90, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 134, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 749, .adv_w = 132, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 758, .adv_w = 202, .box_w = 13, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 780, .adv_w = 136, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 791, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 798, .adv_w = 98, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 808, .adv_w = 77, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 812, .adv_w = 98, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 822, .adv_w = 135, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 3}
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
    -4, 4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -11, 0, -20,
    -11, -26, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, -7, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -22, 0, -31,
    0, -22, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -20, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -13, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -24,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -20, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -13, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -20, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -9, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, -11,
    -13, -11, -9, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -9, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -11, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -17, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, -31, -17,
    -37, -11, -4, 0, -22, 0, 0, -9,
    -13, -7, 0, 0, -9, 0, -7, 0,
    0, 0, 0, 0, 0, -24, -26, -13,
    0, 0, -11, -15, -9, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -31, 0,
    -31, 0, 0, -13, -11, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -13, 0, -9,
    0, -9, 0, 0, 0, 0, 0, -20,
    0, 0, 0, 0, 0, -22, -9, -37,
    0, -33, 0, 0, 0, -4, -7, -7,
    -3, -7, 0, 0, 0, -20, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -13, 0, -13,
    -9, -20, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -9,
    -7, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, -11, 0,
    -9, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, -13,
    -13, -15, -4, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -22, 0,
    -26, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -22, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, -7, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -22,
    0, -20, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -13, 0, -9, 0,
    0, 0, 0, 0, 0, -11, -11, -9,
    0, 0, 0, 0, -9, -9, 0, 0,
    -11, 0, 0, 0, 0, -13, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -26, -7, -31,
    0, -35, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, -9, -22, 0, 0,
    0, 0, 0, 0, 0, 0, -26, 0,
    -35, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -26, 0, -7, 0, 0, -7,
    -17, -9, 0, 0, 0, -7, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -9, -9, -7, -13,
    0, -17, 0, 0, 0, -7, -11, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, -9, 0, 0, -26, -22,
    -24, -7, 0, 0, 0, 0, 0, 0,
    -13, 0, -22, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -17, -26, -13,
    -4, 0, -13, 0, 0, -7, -7, 0,
    0, 0, 0, -9, 0, 0, -28, -13,
    -44, -11, 0, 0, -22, 0, 0, -4,
    0, 0, -35, 0, -9, 0, -7, 0,
    0, 0, 0, 0, 0, -26, -22, 0,
    -3, 0, -11, -15, -9, -9, -9, 0,
    0, 0, -2, -13, 0, 0, -35, -13,
    -37, -13, 0, 0, -13, 0, 0, 0,
    -13, 0, -35, 0, -4, 0, -7, 0,
    0, 0, 0, 0, 0, -22, -22, -9,
    0, 0, -9, -13, -4, 0, -9, 0,
    0, 0, 0, 0, 0, 0, 0, -13,
    0, -13, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, -13, -9,
    0, 0, 0, 0, -13, -13, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, -2, -9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -22, -9, -26,
    0, -31, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -13, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, -13,
    0, -20, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -9,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -13,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, -11, 0,
    -17, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -7, 0,
    -4, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, -20,
    0, 0, 0, -13, 0, 0, -2, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, -9, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -9, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, -9, 0,
    0, 0, 0, 0, 0, 2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -9,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -24, 0,
    -28, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -11, 0, 0, 0, 0, 0,
    -13, -9, 0, 0, 0, -9, -9, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -11,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, -7, 0,
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
const lv_font_t ui_font_NeverMindSemiSerifBold14 = {
#else
lv_font_t ui_font_NeverMindSemiSerifBold14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
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



#endif /*#if UI_FONT_NEVERMINDSEMISERIFBOLD14*/

