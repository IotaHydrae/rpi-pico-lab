// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: wrap_indev

#include "ui.h"

void lv_event_to_dark_theme(lv_event_t * e)
{
	// Your code here
    lv_obj_set_style_bg_color(ui_ScreenHome, lv_color_hsv_to_rgb(225, 71, 7), 0);
    lv_obj_set_style_bg_grad_color(ui_ScreenHome, lv_color_hsv_to_rgb(209, 100, 52), 0);

    lv_obj_set_style_arc_color(ui_SpinnerStatus, lv_color_hsv_to_rgb(209, 100, 35), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}

void lv_event_to_light_theme(lv_event_t * e)
{
	// Your code here
    lv_obj_set_style_bg_color(ui_ScreenHome, lv_color_hsv_to_rgb(4, 72, 60), 0);
    lv_obj_set_style_bg_grad_color(ui_ScreenHome, lv_color_hsv_to_rgb(30, 100, 88), 0);

    lv_obj_set_style_arc_color(ui_SpinnerStatus, lv_color_hsv_to_rgb(357, 58, 51), LV_PART_INDICATOR | LV_STATE_DEFAULT);
}