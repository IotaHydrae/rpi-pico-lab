#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

#include "port/lv_port_disp.h"

void hal_init()
{
	i2c_init(SSD1306_I2C_IF, 1000 * 1000);
	gpio_set_function(SSD1306_PIN_SCL, GPIO_FUNC_I2C);
	gpio_set_function(SSD1306_PIN_SDA, GPIO_FUNC_I2C);
	gpio_pull_up(SSD1306_PIN_SCL);
	gpio_pull_up(SSD1306_PIN_SDA);
}

static void anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

bool lvgl_timer_cb(repeating_timer_t *t)
{
    lv_task_handler();
    return true;
}

extern int i2c_bus_scan(i2c_inst_t *i2c);

int main()
{
	stdio_init_all();

    hal_init();

    sleep_ms(1000);

    i2c_bus_scan(i2c1);

    lv_init();
    lv_port_disp_init();

    lv_demo_stress();
    // lv_demo_benchmark();

    // lv_obj_t *btn = lv_btn_create(lv_scr_act());
    // lv_obj_set_style_bg_color(btn, lv_color_hex(0x0), 0);
    // lv_obj_set_style_radius(btn, 10, 0);
    // lv_obj_center(btn);

    // lv_obj_t *label = lv_label_create(btn);
    // lv_label_set_text(label, "embeddedboys");

    // lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    // lv_anim_t a;
    // lv_anim_init(&a);

    // lv_anim_set_var(&a, btn);
    // lv_anim_set_values(&a, lv_obj_get_y(btn), 5);
    // lv_anim_set_time(&a, 150);
    // lv_anim_set_exec_cb(&a, anim_y_cb);
    // lv_anim_set_path_cb(&a, lv_anim_path_linear);
    // lv_anim_start(&a);


    struct repeating_timer lvgl_timer;
    add_repeating_timer_ms(1, lvgl_timer_cb, NULL, &lvgl_timer);

    for (;;) {
        tight_loop_contents();
        sleep_ms(200);
    }

    return 0;
}
