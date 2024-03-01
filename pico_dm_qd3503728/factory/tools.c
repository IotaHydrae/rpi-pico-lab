
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "tools.h"

LV_FONT_DECLARE(fsex_16);
LV_FONT_DECLARE(fsex_20);

static const lv_font_t *font_normal = &fsex_16;
static const lv_font_t *font_large = &fsex_20;

lv_obj_t *create_label(lv_obj_t *parent, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char *text;
    vasprintf(&text, fmt, args);
    va_end(args);

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, font_normal, 0);

    free(text);

    va_end(args);

    return label;
}

lv_obj_t *create_btn(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_bg_color(btn, lv_color_black(), 0);
    lv_label_set_text(label, txt);
    lv_obj_set_style_text_font(label, &fsex_16, 0);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);
    return btn;
}

lv_obj_t *create_passed_if_btn(lv_obj_t *parent, lv_event_cb_t event_cb, bool *passed)
{
    lv_obj_t * btn_passed = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_passed, event_cb, LV_EVENT_ALL, NULL);
    // lv_obj_align(btn_passed, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_height(btn_passed, LV_SIZE_CONTENT);
    lv_obj_set_user_data(btn_passed, passed);
    lv_obj_set_style_bg_color(btn_passed, lv_palette_main(LV_PALETTE_RED), 0);

    lv_obj_t *label_passed = lv_label_create(btn_passed);
    lv_obj_set_style_text_font(label_passed, &fsex_16, 0);
    lv_label_set_text_fmt(label_passed, "Passed: %s", *passed ? "Yes" : "No");
    lv_obj_center(label_passed);

    return btn_passed;
}