// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.0 Beta1
// LVGL VERSION: 8.2
// PROJECT: epink_desktop

#ifndef _EPINK_DESKTOP_UI_H
#define _EPINK_DESKTOP_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

extern lv_obj_t * ui_ScreenEpinkHome;
extern lv_obj_t * ui_PanelStatusBar;
extern lv_obj_t * ui_LabelTime;
extern lv_obj_t * ui_LabelBattery;
extern lv_obj_t * ui_PanelWidgets;
extern lv_obj_t * ui_RollerHour;
extern lv_obj_t * ui_RollerMinute;
extern lv_obj_t * ui_RollerSecond;
extern lv_obj_t * ui_LabelSplit1;
extern lv_obj_t * ui_LabelSplit2;
extern lv_obj_t * ui_ImageLine;
extern lv_obj_t * ui_ImageLine2;
extern lv_obj_t * ui_LabelHumidity;
extern lv_obj_t * ui_LabelTempture;
extern lv_obj_t * ui_Image4;
extern lv_obj_t * ui_Image5;


LV_IMG_DECLARE(ui_img_line_180_1_png);    // assets\line_180_1.png
LV_IMG_DECLARE(ui_img_humidity_png);    // assets\humidity.png
LV_IMG_DECLARE(ui_img_tempture_png);    // assets\tempture.png


LV_FONT_DECLARE(ui_font_NeverMindSemiSerifBold40);
LV_FONT_DECLARE(ui_font_NeverMindSemiSerifBold72);
LV_FONT_DECLARE(ui_font_NeverMindSemiSerifBold20);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
