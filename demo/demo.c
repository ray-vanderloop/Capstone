/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "msg_queue.h"
#include "packet_process.h"
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <ctype.h>

#if USE_LV_DEMO

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static  lv_res_t btn_click_action(lv_obj_t * btn);



/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t purpleBG;
static lv_style_t grayBG;
static lv_style_t whiteBG;
//static lv_style_t smalltext;
static lv_style_t style_btn_rel;
static lv_style_t style_btn_pr;
static lv_style_t style_bg;

double pi = 3.14150;
 

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a demo application
 */
void demo_create(void)
{

	lv_obj_t * chart;



	//WORK OUT THIS COLOUR ISSUE RIGHT HERE
	//LV_COLOR WESTERN = LV_COLOR_HEX3(0x4F2683);
	//western = LV_COLOR_HEX3(0x4F2683);

	//lv_cont_set_fit(lv_scr_act(), true, true);
	//lv_obj_set_size(lv_scr_act(), 720, 480);
	//lv_cont_fit(lv_scr_act(), LV_FIT_TIGHT);

	//Create styles
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
	style_btn_rel.body.empty = 1;                  
	style_btn_rel.body.border.color = LV_COLOR_WHITE;
	style_btn_rel.body.border.width = 2;
	style_btn_rel.body.border.opa = LV_OPA_70;
	style_btn_rel.body.padding.hor = 12;
	style_btn_rel.body.padding.ver = 8;

	lv_style_copy(&style_btn_pr, &style_btn_rel);
	style_btn_pr.body.empty = 0;
	style_btn_pr.body.main_color = LV_COLOR_PURPLE;
	style_btn_pr.body.grad_color = LV_COLOR_PURPLE;


	//smalltext.text.font = &lv_font_dejavu_20;


	//create outer container
	lv_obj_t * window = lv_cont_create(lv_scr_act(),NULL);
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


	//Add a text to the container
	lv_obj_t * flighttimetext = lv_label_create(box1, NULL); 
	lv_obj_set_style(box1, &purpleBG);
	lv_label_set_text(flighttimetext, "Flight Time");

	char fttemp[50];
	snprintf(fttemp, 30, "%i", psensor.accelmeter);


	lv_obj_align(box1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * box7;
	box7 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box7, &purpleBG);
	lv_cont_set_fit(box7, true, true);


	//Add a text to the container
	lv_obj_t * flighttimevalue = lv_label_create(box7, NULL); 
	lv_obj_set_style(box7, &purpleBG);
	lv_label_set_text(flighttimevalue, fttemp);

	char maxtemp[50];
	snprintf(maxtemp, 30, "%i", psensor.accelmeter);


	lv_obj_align(box7, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 0);









	lv_obj_t * box2;
	box2 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box2, &grayBG);
	lv_cont_set_fit(box2, true, true);


	lv_obj_t * maxtext = lv_label_create(box2, NULL); 
	lv_label_set_text(maxtext, "Max Speed: ");


	lv_obj_align(box2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 35);
	
	lv_obj_t * box8;
	box8 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box8, &grayBG);
	lv_cont_set_fit(box8, true, true);


	lv_obj_t * maxvalue = lv_label_create(box8, NULL); 
	lv_label_set_text(maxvalue, "Label");


	lv_obj_align(box8, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 35);
	
	
	
	
	
	
	
	

	lv_obj_t * box3;
	box3 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box3, &purpleBG);
	lv_cont_set_fit(box3, true, true);


	//Add a text to the container
	lv_obj_t * avgspeedtext = lv_label_create(box3, NULL);
	lv_label_set_text(avgspeedtext, "Avg Speed: ");

	lv_obj_align(box3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 70);


	lv_obj_t * box9;
	box9 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box9, &purpleBG);
	lv_cont_set_fit(box9, true, true);


	//Add a text to the container
	lv_obj_t * avgspeedvalue = lv_label_create(box9, NULL);
	lv_label_set_text(avgspeedvalue, "Text");

	lv_obj_align(box9, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 70);








	lv_obj_t * box4;
	box4 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box4, &grayBG);
	lv_cont_set_fit(box4, true, true);

	//Add a text to the container
	lv_obj_t * droguetext = lv_label_create(box4, NULL);
	lv_label_set_text(droguetext, "Drogue Time: ");

	lv_obj_align(box4, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 105);
	
	lv_obj_t * box10;
	box10 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box10, &grayBG);
	lv_cont_set_fit(box10, true, true);

	//Add a text to the container
	lv_obj_t * droguevalue = lv_label_create(box10, NULL);
	lv_label_set_text(droguevalue, "Text");

	lv_obj_align(box10, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 105);
	
	
	
	
	
	


	lv_obj_t * box5;
	box5 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box5, &purpleBG);
	lv_cont_set_fit(box5, true, true);

	//Add a text to the container
	lv_obj_t * maintext= lv_label_create(box5, NULL);
	lv_label_set_text(maintext, "Main Time: ");

	lv_obj_align(box5, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 140);
	
	lv_obj_t * box11;
	box11 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box11, &purpleBG);
	lv_cont_set_fit(box11, true, true);

	//Add a text to the container
	lv_obj_t * mainvalue= lv_label_create(box11, NULL);
	lv_label_set_text(mainvalue, "Main Time: ");

	lv_obj_align(box11, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 140);
	
	
	
	
	
	
	
	
	


	lv_obj_t * box6;
	box6 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box6, &grayBG);
	lv_cont_set_fit(box6, true, true);


	//Add a text to the container
	lv_obj_t * apogeetext = lv_label_create(box6, NULL);
	lv_label_set_text(apogeetext, "Apogee Time: ");

	lv_obj_align(box6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 175);
	
	lv_obj_t * box12;
	box12 = lv_cont_create(rightside, NULL);
	lv_obj_set_style(box12, &grayBG);
	lv_cont_set_fit(box12, true, true);


	//Add a text to the container
	lv_obj_t * apogeevalue = lv_label_create(box12, NULL);
	lv_label_set_text(apogeevalue, "Apogee Time: ");

	lv_obj_align(box12, NULL, LV_ALIGN_IN_TOP_LEFT, 120, 175);



	/*Create a chart*/
	chart = lv_chart_create(window, NULL);
	lv_obj_set_size(chart, 250, 210);
	lv_obj_set_style(chart, NULL);
	lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_chart_set_type(chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);   /*Show lines and points too*/
	lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
	lv_chart_set_series_width(chart, 4);                                  /*Line width and point radius*/

	lv_chart_set_range(chart, -1000, 11000);
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
	lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action); /*Set function to be called when the button is released*/
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
 
 
 
 
static  lv_res_t btn_click_action(lv_obj_t * btn)
{
	lv_obj_t * mbox1 = lv_mbox_create(lv_scr_act(), NULL);
	lv_mbox_set_text(mbox1, "Dropped Packets");
	lv_cont_set_fit(mbox1, false, false);
	lv_obj_set_size(mbox1, 400, 200);
	lv_obj_align(mbox1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

	lv_obj_t * m1 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m1, &whiteBG);
	lv_obj_set_size(m1, 400, 200);
	lv_cont_set_fit(m1, false, false);
	lv_obj_align(m1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	lv_obj_t * m2 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m2, &purpleBG);
	lv_obj_set_size(m2, 100, 25);
	lv_cont_set_fit(m2, false, false);
	lv_obj_align(m2, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 0);

	lv_obj_t * m3 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m3, &purpleBG);
	lv_obj_set_size(m3, 200, 25);
	lv_cont_set_fit(m3, false, false);
	lv_obj_align(m3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 50);
	
	lv_obj_t * m4 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m4, &purpleBG);
	lv_obj_set_size(m4, 100, 25);
	lv_cont_set_fit(m4, false, false);
	lv_obj_align(m4, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 100);

	lv_obj_t * m5 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m5, &purpleBG);
	lv_obj_set_size(m5, 200, 25);
	lv_cont_set_fit(m5, false, false);
	lv_obj_align(m5, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 50);
	
	lv_obj_t * m6 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m6, &purpleBG);
	lv_obj_set_size(m6, 100, 25);
	lv_cont_set_fit(m6, false, false);
	lv_obj_align(m6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 100);

	lv_obj_t * m7 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m7, &purpleBG);
	lv_obj_set_size(m7, 200, 25);
	lv_cont_set_fit(m7, false, false);
	lv_obj_align(m7, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 150);
	
	lv_obj_t * m8 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m8, &purpleBG);
	lv_obj_set_size(m8, 100, 25);
	lv_cont_set_fit(m8, false, false);
	lv_obj_align(m8, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 150);

	lv_obj_t * m9 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m9, &purpleBG);
	lv_obj_set_size(m9, 200, 25);
	lv_cont_set_fit(m9, false, false);
	lv_obj_align(m9, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 200);
	
	lv_obj_t * m10 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m10, &purpleBG);
	lv_obj_set_size(m10, 100, 25);
	lv_cont_set_fit(m10, false, false);
	lv_obj_align(m10, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 200);

	lv_obj_t * m11 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m11, &purpleBG);
	lv_obj_set_size(m11, 200, 25);
	lv_cont_set_fit(m11, false, false);
	lv_obj_align(m11, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 250);
	
	lv_obj_t * m12 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m12, &purpleBG);
	lv_obj_set_size(m12, 100, 25);
	lv_cont_set_fit(m12, false, false);
	lv_obj_align(m12, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 250);

	lv_obj_t * m13 = lv_cont_create(mbox1, NULL);
	lv_obj_set_style(m13, &purpleBG);
	lv_obj_set_size(m13, 200, 25);
	lv_cont_set_fit(m13, false, false);
	lv_obj_align(m13, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 300);		







	lv_obj_t * l2 = lv_label_create(m2, NULL);
	lv_label_set_text(l2, "Total Dropped:");
	lv_obj_align(l2, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l3 = lv_label_create(m3, NULL);
	lv_label_set_text(l3, "Total Dropped:");
	lv_obj_align(l3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l4 = lv_label_create(m4, NULL);
	lv_label_set_text(l4, "Total Dropped:");
	lv_obj_align(l4, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l5 = lv_label_create(m5, NULL);
	lv_label_set_text(l5, "Total Dropped:");
	lv_obj_align(l5, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l6 = lv_label_create(m6, NULL);
	lv_label_set_text(l6, "Total Dropped:");
	lv_obj_align(l6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l7 = lv_label_create(m7, NULL);
	lv_label_set_text(l7, "Total Dropped:");
	lv_obj_align(l7, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l8 = lv_label_create(m8, NULL);
	lv_label_set_text(l8, "Total Dropped:");
	lv_obj_align(l8, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l9 = lv_label_create(m9, NULL);
	lv_label_set_text(l9, "Total Dropped:");
	lv_obj_align(l9, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l10 = lv_label_create(m10, NULL);
	lv_label_set_text(l10, "Total Dropped:");
	lv_obj_align(l10, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l11 = lv_label_create(m11, NULL);
	lv_label_set_text(l11, "Total Dropped:");
	lv_obj_align(l11, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l12 = lv_label_create(m12, NULL);
	lv_label_set_text(l12, "Total Dropped:");
	lv_obj_align(l12, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	
	lv_obj_t * l13 = lv_label_create(m13, NULL);
	lv_label_set_text(l13, "Total Dropped:");
	lv_obj_align(l13, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    return LV_RES_OK;
}



#endif  /*USE_LV_DEMO*/
