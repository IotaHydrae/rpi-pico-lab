#include <string.h>
#include "lvgl/lvgl.h"

#include "backlight.h"

#define LV_PRId32 PRId32

static lv_obj_t *scr_home;
static lv_obj_t *scr_backlight;
static lv_obj_t *scr_touch;
static lv_obj_t *scr_stress;

static lv_obj_t * btnm1_home;

static const lv_font_t * font_normal = LV_FONT_DEFAULT;

static bool test_bl_passed = false;
static bool test_touch_passed = false;
static bool test_stress_passed = false;

static void btn_home_event_handler(lv_event_t * e)
{
    lv_scr_load(scr_home);

    lv_event_send(btnm1_home, LV_EVENT_SCREEN_LOADED, NULL);
}

static void slider_backlight_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t *lbl = lv_obj_get_user_data(obj);

    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        lv_coord_t * s = lv_event_get_param(e);
        *s = LV_MAX(*s, 60);
    }
    else if(code == LV_EVENT_DRAW_PART_END) {
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

static void btn_passed_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * obj = lv_event_get_target(e);
    bool *passed = lv_obj_get_user_data(obj);
    lv_obj_t *lbl = lv_obj_get_child(obj, 0);

    if(code == LV_EVENT_PRESSED){

        *passed = !*passed;
        if(*passed) {
            lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), 0);
        } else {
            lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
        }
        lv_label_set_text_fmt(lbl, "Passed: %s", *passed ? "Yes" : "No");
    }
    
    
}

static lv_obj_t *create_btn(lv_obj_t *parent, const char *txt, lv_event_cb_t event_cb)
{
    lv_obj_t *btn_home = lv_btn_create(parent);
    lv_obj_t *label = lv_label_create(btn_home);
    lv_label_set_text(label, LV_SYMBOL_HOME);
    lv_obj_add_event_cb(btn_home, btn_home_event_handler, LV_EVENT_PRESSED, NULL);
    return btn_home;
}

static lv_obj_t *create_passed_if_btn(lv_obj_t *parent, lv_event_cb_t event_cb, bool *passed)
{
    lv_obj_t * btn_passed = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_passed, event_cb, LV_EVENT_ALL, NULL);
    // lv_obj_align(btn_passed, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_height(btn_passed, LV_SIZE_CONTENT);
    lv_obj_set_user_data(btn_passed, passed);
    lv_obj_set_style_bg_color(btn_passed, lv_palette_main(LV_PALETTE_RED), 0);

    lv_obj_t *label_passed = lv_label_create(btn_passed);
    lv_label_set_text_fmt(label_passed, "Passed: %s", test_bl_passed ? "Yes" : "No");
    lv_obj_center(label_passed);

    return btn_passed;
}

static int scr_backlight_init(void)
{
    scr_backlight = lv_obj_create(NULL);

    lv_obj_t *title_label = lv_label_create(scr_backlight);
    lv_label_set_text(title_label, "Backlight Test");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    /* backlight controller slider */
    lv_obj_t * slider1 = lv_slider_create(scr_backlight);
    lv_slider_set_range(slider1, 0, 100);
    lv_slider_set_value(slider1, 100, LV_ANIM_ON);
    lv_obj_align(slider1, LV_ALIGN_CENTER, 0, 20);
    lv_obj_add_event_cb(slider1, slider_backlight_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t *label_slider_val = lv_label_create(scr_backlight);
    lv_label_set_text_fmt(label_slider_val, "Backlight level : %d", lv_slider_get_value(slider1));
    lv_obj_set_style_text_font(label_slider_val, &lv_font_montserrat_16, 0);
    lv_obj_align(label_slider_val, LV_ALIGN_CENTER, 0, -40);

    lv_obj_set_user_data(slider1, label_slider_val);

    /* a passed-if btn create here */
    lv_obj_t *btn_passed = create_passed_if_btn(scr_backlight, btn_passed_event_handler, &test_bl_passed);
    lv_obj_align(btn_passed, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* create return-to-home button */
    lv_obj_t *btn_home = create_btn(scr_backlight, LV_SYMBOL_HOME, btn_home_event_handler);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);

    return 0;
}

static int scr_touch_init(void)
{
    scr_touch = lv_obj_create(NULL);

    lv_obj_t *title_label = lv_label_create(scr_touch);
    lv_label_set_text(title_label, "Touch Test");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    /* a passed-if btn create here */
    lv_obj_t *btn_passed = create_passed_if_btn(scr_touch, btn_passed_event_handler, &test_touch_passed);
    lv_obj_align(btn_passed, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* create return-to-home button */
    lv_obj_t *btn_home = create_btn(scr_touch, LV_SYMBOL_HOME, btn_home_event_handler);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);
    return 0;
}

static int scr_stress_init(void)
{
    scr_stress = lv_obj_create(NULL);

    lv_obj_t *title_label = lv_label_create(scr_stress);
    lv_label_set_text(title_label, "Stress Test");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    /* a passed-if btn create here */
    lv_obj_t *btn_passed = create_passed_if_btn(scr_stress, btn_passed_event_handler, &test_stress_passed);
    lv_obj_align(btn_passed, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* create return-to-home button */
    lv_obj_t *btn_home = create_btn(scr_stress, LV_SYMBOL_HOME, btn_home_event_handler);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);
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
            } else if (dsc->id == 1 && test_touch_passed) {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_GREEN);
            } else if (dsc->id == 2 && test_stress_passed) {
                dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_GREEN);
            }
        }
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);
        if (strcmp(txt, "Backlight") == 0) {
            lv_scr_load(scr_backlight);
        }
        else if (strcmp(txt, "Touch") == 0) {
            LV_LOG_USER("Touch was pressed\n");
            lv_scr_load(scr_touch);
        }
        else if (strcmp(txt, "Stress") == 0) {
            LV_LOG_USER("Stress was pressed\n");
            lv_scr_load(scr_stress);
        }
    }
}

static const char * btnm_map[] = {
    "Backlight", "Touch", "Stress", "",
};

int factory_test(void)
{
    /* initialize screens here */
    scr_home = lv_obj_create(NULL);
    lv_scr_load(scr_home);
    
    scr_backlight_init();
    scr_touch_init();
    scr_stress_init();

    lv_obj_t *label = lv_label_create(scr_home);
    lv_label_set_text(label, "Factory Test Suit");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);

    btnm1_home = lv_btnmatrix_create(scr_home);
    lv_btnmatrix_set_map(btnm1_home, btnm_map);
    lv_obj_set_size(btnm1_home, 460, 200);
    lv_obj_set_style_text_font(btnm1_home, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(btnm1_home, btnmatrix_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_center(btnm1_home);

    lv_obj_t *btn_home = create_btn(scr_home, LV_SYMBOL_HOME, btn_home_event_handler);
    lv_obj_align(btn_home, LV_ALIGN_BOTTOM_MID, 0, -10);

    return 0;
}