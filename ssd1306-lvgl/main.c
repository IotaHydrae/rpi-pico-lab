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
	i2c_init(i2c_default, 400 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	ssd1306_init();
}

static void anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

int main()
{
	stdio_init_all();

    hal_init();

    lv_init();
    lv_port_disp_init();

    printf("%s\n", __func__);

    // lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_music();

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x0), 0);
    lv_obj_set_style_radius(btn, 10, 0);
    lv_obj_center(btn);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "embeddedboys");

    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    lv_anim_t a;
    lv_anim_init(&a);

    lv_anim_set_var(&a, btn);
    lv_anim_set_values(&a, lv_obj_get_y(btn), 20);
    lv_anim_set_time(&a, 200);
    lv_anim_set_exec_cb(&a, anim_y_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);

    while( 1 ) {
        sleep_us(5*1000);
        lv_timer_handler();
        lv_tick_inc(5);
    }
    
    return 0;
}
