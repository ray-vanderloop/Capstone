/*
 * This is the final GUI for the WE Rocketry Team
 * any questions please email Rachel Vanderloop
 * rvanderl@uwo.ca or rvanderloop332@gmail.com if no response
 * good luck my children <3
 */

/*********************
 *      INCLUDES
 *********************/
#include "final.h"
#if USE_LV_TUTORIALS

/*********************
 *      DEFINES
 *********************/

static lv_style_t purpleBG;
static lv_style_t grayBG;
static lv_style_t whiteBG;
static lv_style_t smalltext;
static lv_style_t style_btn_rel;
static lv_style_t style_btn_pr;
static lv_style_t style_bg;

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static  lv_res_t btn_rel_action(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/




void final(void)
{

	lv_obj_t * chart;

	//WORK OUT THIS COLOUR ISSUE RIGHT HERE
	//LV_COLOR WESTERN = LV_COLOR_HEX3(0x4F2683);
	//western = LV_COLOR_HEX3(0x4F2683);

	/*Create styles*/
	lv_style_copy(&purpleBG, &lv_style_plain);
	purpleBG.body.main_color = LV_COLOR_PURPLE;
	purpleBG.body.grad_color = LV_COLOR_PURPLE;
	purpleBG.text.color = LV_COLOR_WHITE;

	lv_style_copy(&grayBG, &lv_style_plain);
	grayBG.body.main_color = LV_COLOR_GRAY;
	grayBG.body.grad_color = LV_COLOR_GRAY;

	lv_style_copy(&whiteBG, &lv_style_plain);
	whiteBG.body.main_color = LV_COLOR_WHITE;
	whiteBG.body.grad_color = LV_COLOR_WHITE;

	lv_style_copy(&style_bg, &lv_style_pretty);
	style_bg.body.main_color = LV_COLOR_PURPLE;
	style_bg.body.grad_color = LV_COLOR_PURPLE;
	style_bg.body.border.color = LV_COLOR_PURPLE;
	style_bg.text.color = LV_COLOR_WHITE;
	style_bg.body.padding.hor = 12;
	style_bg.body.padding.ver = 8;
	style_bg.body.shadow.width = 8;


	lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
	style_btn_rel.body.empty = 1;                    /*Draw only the border*/
	style_btn_rel.body.border.color = LV_COLOR_WHITE;
	style_btn_rel.body.border.width = 2;
	style_btn_rel.body.border.opa = LV_OPA_70;
	style_btn_rel.body.padding.hor = 12;
	style_btn_rel.body.padding.ver = 8;

	lv_style_copy(&style_btn_pr, &style_btn_rel);
	style_btn_pr.body.empty = 0;
	style_btn_pr.body.main_color = LV_COLOR_PURPLE;
	style_btn_pr.body.grad_color = LV_COLOR_PURPLE;


	smalltext.text.font = &lv_font_dejavu_20;


	//lv_cont_set_fit(lv_scr_act(), false, false);
	//lv_obj_set_size(lv_scr_act(), LV_HOR_RES, LV_VER_RES);
	//lv_cont_fit(lv_scr_act(), LV_FIT_TIGHT);



	//create outer container
	lv_obj_t * window = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_size(window, 720, 480);
    lv_win_set_style(window, LV_WIN_STYLE_BG, &whiteBG);
	lv_obj_align(window, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);
	lv_cont_set_fit(window, false, false);

	//create right hand container for labels

	lv_obj_t * rightside;
	rightside = lv_cont_create(lv_scr_act(), NULL);
    lv_win_set_style(rightside, LV_WIN_STYLE_BG, &whiteBG);
	lv_obj_align(rightside, NULL, LV_ALIGN_IN_TOP_LEFT, 260, 5);
	lv_cont_set_fit(rightside, false, false);
	lv_obj_set_size(rightside, 450, 210);

	/*
	 * create containers and their text labels
	 * */

	lv_obj_t * box1;
	box1 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box1, &purpleBG);
	lv_cont_set_fit(box1, true, true);

	/*Add a text to the container*/
	lv_obj_t * flighttimelabel = lv_label_create(box1, NULL);
	lv_label_set_text(flighttimelabel, "Flight Time: ");

	lv_obj_align(box1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);


	lv_obj_t * box2;
	box2 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box2, &grayBG);
	lv_cont_set_fit(box2, true, true);

	/*Add a text to the container*/
	lv_obj_t * maxspeedlabel = lv_label_create(box2, NULL);
	lv_label_set_text(maxspeedlabel, "Max Speed: ");

	lv_obj_align(box2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 35);


	lv_obj_t * box3;
	box3 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box3, &purpleBG);
	lv_cont_set_fit(box3, true, true);

	/*Add a text to the container*/
	lv_obj_t * avgspeedlabel = lv_label_create(box3, NULL);
	lv_label_set_text(avgspeedlabel, "Average Speed: ");

	lv_obj_align(box3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 70);


	lv_obj_t * box4;
	box4 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box4, &grayBG);
	lv_cont_set_fit(box4, true, true);

	/*Add a text to the container*/
	lv_obj_t * droguelabel = lv_label_create(box4, NULL);
	lv_label_set_text(droguelabel, "Drogue Time: ");

	lv_obj_align(box4, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 105);


	lv_obj_t * box5;
	box5 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box5, &purpleBG);
	lv_cont_set_fit(box5, true, true);

	/*Add a text to the container*/
	lv_obj_t * mainlabel = lv_label_create(box5, NULL);
	lv_label_set_text(mainlabel, "Main Time: ");

	lv_obj_align(box5, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 140);


	lv_obj_t * box6;
	box6 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box6, &grayBG);
	lv_cont_set_fit(box6, true, true);

	/*Add a text to the container*/
	lv_obj_t * apogeelabel = lv_label_create(box6, NULL);
	lv_label_set_text(apogeelabel, "Apogee Time: ");

	lv_obj_align(box6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 175);



	/*Create a chart*/
	chart = lv_chart_create(window, NULL);
	lv_obj_set_size(chart, 250, 210);
	lv_obj_set_style(chart, NULL);
	lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_chart_set_type(chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);   /*Show lines and points too*/
	lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
	lv_chart_set_series_width(chart, 4);                                  /*Line width and point radius*/

	lv_chart_set_range(chart, -1000, 11000);
	//lv_chart_set_domain(chart, y_min, y_max);
	lv_chart_set_point_count(chart, 6);

	/*Add two data series*/
	lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_PURPLE);


	/*Directly set points on 'dl2'*/
	ser1->points[0] = 0;
	ser1->points[1] = 0;
	ser1->points[2] = 0;
	ser1->points[3] = 10000;
	ser1->points[4] = 9500;
	ser1->points[5] = 3000;
	ser1->points[5] = 0;


	lv_chart_refresh(chart);


	lv_obj_t * bottom = lv_cont_create(window, NULL);
	lv_obj_set_size(bottom, 720, 200);
    lv_win_set_style(bottom, LV_WIN_STYLE_BG, &whiteBG);
	lv_obj_align(bottom, NULL, LV_ALIGN_IN_LEFT_MID, 0, 70);
	lv_cont_set_fit(bottom, false, false);


	lv_obj_t * bbox1;
	bbox1 = lv_cont_create(bottom, NULL);
	lv_obj_set_style(bbox1, &grayBG);
	lv_obj_set_size(bbox1, 255, 50);
	lv_cont_set_fit(bbox1, false, false);
	lv_obj_align(bbox1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);



	//add text to container
	lv_obj_t * gpslabel = lv_label_create(bbox1, NULL);
	lv_label_set_text(gpslabel, "GPS Coordinates: \n\n");
	lv_obj_align(gpslabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 15);



    lv_obj_t * btn1 = lv_btn_create(bottom, NULL);         /*Create a button on the currently loaded screen*/
    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_rel_action); /*Set function to be called when the button is released*/
	lv_obj_align(btn1, gpslabel, LV_ALIGN_IN_TOP_LEFT, 300, 0);

	lv_btn_set_style(btn1, LV_MBOX_STYLE_BG, &style_bg);
	lv_btn_set_style(btn1, LV_MBOX_STYLE_BTN_REL, &style_btn_rel);
	lv_btn_set_style(btn1, LV_MBOX_STYLE_BTN_PR, &style_btn_pr);



    lv_obj_t * buttonlabel = lv_label_create(btn1, NULL);
    lv_label_set_text(buttonlabel, "Packet Info");




}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static  lv_res_t btn_rel_action(void)
{
	lv_obj_t * mbox1 = lv_mbox_create(lv_scr_act(), NULL);
	lv_mbox_set_text(mbox1, "Dropped Packets");
	lv_cont_set_fit(mbox1, false, false);
	lv_obj_set_size(mbox1, 400, 200);
	lv_obj_align(mbox1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

	lv_obj_t * m1 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m1, &whiteBG);
	lv_obj_set_size(m1, 400, 198);
	lv_cont_set_fit(m1, false, false);
	lv_obj_align(m1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * m2 = lv_cont_create(m1, NULL);
	lv_obj_set_style(m2, &purpleBG);
	lv_obj_set_size(m2, 200, 25);
	lv_cont_set_fit(m2, false, false);
	lv_obj_align(m2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * l2 = lv_label_create(m2, NULL);
	lv_label_set_text(l2, "Total Dropped: \n\n");
	lv_obj_align(l2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    return LV_RES_OK;
}

#endif /*USE_LV_TUTORIALS*/
