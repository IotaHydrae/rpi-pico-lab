// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.0 Beta1
// LVGL VERSION: 8.2
// PROJECT: epink_desktop

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_ScreenEpinkHome;
lv_obj_t * ui_PanelStatusBar;
lv_obj_t * ui_LabelTime;
lv_obj_t * ui_LabelBattery;
lv_obj_t * ui_PanelWidgets;
lv_obj_t * ui_RollerHour;
lv_obj_t * ui_RollerMinute;
lv_obj_t * ui_RollerSecond;
lv_obj_t * ui_LabelSplit1;
lv_obj_t * ui_LabelSplit2;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 8
    #error "LV_COLOR_DEPTH should be 8bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_ScreenEpinkHome_screen_init(void)
{
    ui_ScreenEpinkHome = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenEpinkHome, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_PanelStatusBar = lv_obj_create(ui_ScreenEpinkHome);
    lv_obj_set_width(ui_PanelStatusBar, 190);
    lv_obj_set_height(ui_PanelStatusBar, 20);
    lv_obj_set_x(ui_PanelStatusBar, 0);
    lv_obj_set_y(ui_PanelStatusBar, 5);
    lv_obj_set_align(ui_PanelStatusBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_PanelStatusBar, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PanelStatusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelStatusBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTime = lv_label_create(ui_PanelStatusBar);
    lv_obj_set_width(ui_LabelTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTime, 5);
    lv_obj_set_y(ui_LabelTime, 0);
    lv_obj_set_align(ui_LabelTime, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_LabelTime, "14:20");
    lv_obj_set_style_text_color(ui_LabelTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelTime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelTime, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBattery = lv_label_create(ui_PanelStatusBar);
    lv_obj_set_width(ui_LabelBattery, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBattery, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBattery, -5);
    lv_obj_set_y(ui_LabelBattery, 0);
    lv_obj_set_align(ui_LabelBattery, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_LabelBattery, "100%");
    lv_obj_set_style_text_color(ui_LabelBattery, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBattery, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelWidgets = lv_obj_create(ui_ScreenEpinkHome);
    lv_obj_set_width(ui_PanelWidgets, 190);
    lv_obj_set_height(ui_PanelWidgets, 160);
    lv_obj_set_x(ui_PanelWidgets, 0);
    lv_obj_set_y(ui_PanelWidgets, 15);
    lv_obj_set_align(ui_PanelWidgets, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_PanelWidgets, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_RollerHour = lv_roller_create(ui_PanelWidgets);
    lv_roller_set_options(ui_RollerHour,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_RollerHour, 46);
    lv_obj_set_width(ui_RollerHour, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_RollerHour, 5);
    lv_obj_set_y(ui_RollerHour, 0);
    lv_obj_set_align(ui_RollerHour, LV_ALIGN_LEFT_MID);
    lv_obj_set_style_text_color(ui_RollerHour, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerHour, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerHour, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerHour, &ui_font_NeverMindSemiSerifBold40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(ui_RollerHour, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerHour, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerHour, LV_TEXT_ALIGN_CENTER, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerHour, &ui_font_NeverMindSemiSerifBold40, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RollerHour, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RollerHour, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_RollerMinute = lv_roller_create(ui_PanelWidgets);
    lv_roller_set_options(ui_RollerMinute,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_RollerMinute, 46);
    lv_obj_set_width(ui_RollerMinute, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align(ui_RollerMinute, LV_ALIGN_CENTER);
    lv_obj_set_style_text_color(ui_RollerMinute, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerMinute, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerMinute, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerMinute, &ui_font_NeverMindSemiSerifBold40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(ui_RollerMinute, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerMinute, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerMinute, LV_TEXT_ALIGN_CENTER, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerMinute, &ui_font_NeverMindSemiSerifBold40, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RollerMinute, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RollerMinute, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_RollerSecond = lv_roller_create(ui_PanelWidgets);
    lv_roller_set_options(ui_RollerSecond,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_RollerSecond, 46);
    lv_obj_set_width(ui_RollerSecond, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_RollerSecond, -5);
    lv_obj_set_y(ui_RollerSecond, 0);
    lv_obj_set_align(ui_RollerSecond, LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_text_color(ui_RollerSecond, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerSecond, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerSecond, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerSecond, &ui_font_NeverMindSemiSerifBold40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(ui_RollerSecond, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_RollerSecond, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_RollerSecond, LV_TEXT_ALIGN_CENTER, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RollerSecond, &ui_font_NeverMindSemiSerifBold40, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_RollerSecond, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_RollerSecond, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_LabelSplit1 = lv_label_create(ui_PanelWidgets);
    lv_obj_set_width(ui_LabelSplit1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelSplit1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSplit1, -33);
    lv_obj_set_y(ui_LabelSplit1, -5);
    lv_obj_set_align(ui_LabelSplit1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSplit1, ":");
    lv_obj_set_style_text_color(ui_LabelSplit1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelSplit1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelSplit1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelSplit1, &ui_font_NeverMindSemiSerifBold40, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelSplit2 = lv_label_create(ui_PanelWidgets);
    lv_obj_set_width(ui_LabelSplit2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelSplit2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSplit2, 31);
    lv_obj_set_y(ui_LabelSplit2, -5);
    lv_obj_set_align(ui_LabelSplit2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSplit2, ":");
    lv_obj_set_style_text_color(ui_LabelSplit2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelSplit2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelSplit2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelSplit2, &ui_font_NeverMindSemiSerifBold40, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_basic_init(dispp);
    lv_disp_set_theme(dispp, theme);
    ui_ScreenEpinkHome_screen_init();
    lv_disp_load_scr(ui_ScreenEpinkHome);
}
