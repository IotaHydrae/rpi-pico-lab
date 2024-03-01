#pragma once

#ifndef __FACTORY_TOOLS_H
#define __FACTORY_TOOLS_H

#include "lvgl/lvgl.h"

lv_obj_t *create_label(lv_obj_t *parent, const char *fmt, ...);
lv_obj_t *create_btn(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb);
lv_obj_t *create_passed_if_btn(lv_obj_t *parent, lv_event_cb_t event_cb, bool *passed);

#endif