/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xvtc.h"
#include "vga_modes.h"
#include "xv_tpg.h"
#include "xvidc.h"
#include "xv_mix.h"
#include "xhud_gen.h"
#include "xgpio.h"

XVtc	VtcInst;
XVtc_Config *vtc_config ;
XV_tpg tpg;
XV_tpg_Config *tpg_config;
XGpio_Config *gpio_config;
XGpio gpio;

VideoMode video;

int main()
{
	XVtc_Timing vtcTiming;
	XVtc_SourceSelect SourceSelect;
    XV_mix xv_mix;
    XV_mix_Config *xv_config;

    XHud_gen_Config *XV_Hud_cfg;
	XHud_gen xv_hud;

	init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application");

    gpio_config = XGpio_LookupConfig(XPAR_GPIO_0_DEVICE_ID);
    XGpio_CfgInitialize(&gpio,gpio_config, gpio_config->BaseAddress);

    XGpio_SetDataDirection(&gpio,1,0xFFFFFFFF);
    XGpio_SetDataDirection(&gpio,2,0x00000000);

    vtc_config = XVtc_LookupConfig(XPAR_VTC_0_DEVICE_ID);
    XVtc_CfgInitialize(&VtcInst, vtc_config, vtc_config->BaseAddress);


    video = VMODE_640x480;
	vtcTiming.HActiveVideo = video.width;	/**< Horizontal Active Video Size */
	vtcTiming.HFrontPorch = video.hps - video.width;	/**< Horizontal Front Porch Size */
	vtcTiming.HSyncWidth = video.hpe - video.hps;		/**< Horizontal Sync Width */
	vtcTiming.HBackPorch = video.hmax - video.hpe + 1;		/**< Horizontal Back Porch Size */
	vtcTiming.HSyncPolarity = video.hpol;	/**< Horizontal Sync Polarity */
	vtcTiming.VActiveVideo = video.height;	/**< Vertical Active Video Size */
	vtcTiming.V0FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V0SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V0BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.V1FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V1SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V1BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.VSyncPolarity = video.vpol;	/**< Vertical Sync Polarity */
	vtcTiming.Interlaced = 0;

    memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
	SourceSelect.VBlankPolSrc = 1;
	SourceSelect.VSyncPolSrc = 1;
	SourceSelect.HBlankPolSrc = 1;
	SourceSelect.HSyncPolSrc = 1;
	SourceSelect.ActiveVideoPolSrc = 1;
	SourceSelect.ActiveChromaPolSrc= 1;
	SourceSelect.VChromaSrc = 1;
	SourceSelect.VActiveSrc = 1;
	SourceSelect.VBackPorchSrc = 1;
	SourceSelect.VSyncSrc = 1;
	SourceSelect.VFrontPorchSrc = 1;
	SourceSelect.VTotalSrc = 1;
	SourceSelect.HActiveSrc = 1;
	SourceSelect.HBackPorchSrc = 1;
	SourceSelect.HSyncSrc = 1;
	SourceSelect.HFrontPorchSrc = 1;
	SourceSelect.HTotalSrc = 1;
	XVtc_RegUpdateEnable(&VtcInst);
	XVtc_SetGeneratorTiming(&VtcInst, &vtcTiming);
	XVtc_SetSource(&VtcInst, &SourceSelect);
	XVtc_EnableGenerator(&VtcInst);

	xv_config = XV_mix_LookupConfig(XPAR_XV_MIX_0_DEVICE_ID);

	XV_mix_CfgInitialize(&xv_mix,xv_config,xv_config->BaseAddress);

	XV_mix_Set_HwReg_width(&xv_mix, (u32)640);
	XV_mix_Set_HwReg_height(&xv_mix, (u32) 480);
	XV_mix_Set_HwReg_layerEnable(&xv_mix,(u32)3);

	XV_mix_Set_HwReg_layerStartX_0(&xv_mix,(u32)0);
	XV_mix_Set_HwReg_layerStartY_0(&xv_mix,0);
	XV_mix_Set_HwReg_layerWidth_0(&xv_mix,(u32)640);
	XV_mix_Set_HwReg_layerHeight_0(&xv_mix,(u32)480);
	XV_mix_Set_HwReg_layerAlpha_0(&xv_mix, 225);

	XV_mix_Set_HwReg_layerStartX_1(&xv_mix,(u32)0);
	XV_mix_Set_HwReg_layerStartY_1(&xv_mix,0);
	XV_mix_Set_HwReg_layerWidth_1(&xv_mix,(u32)640);
	XV_mix_Set_HwReg_layerHeight_1(&xv_mix,(u32)480);
	XV_mix_Set_HwReg_layerAlpha_1(&xv_mix, 250);

	XV_mix_EnableAutoRestart(&xv_mix);
	XV_mix_Start(&xv_mix);

	XV_Hud_cfg = XHud_gen_LookupConfig(XPAR_HUD_GEN_0_DEVICE_ID);
	XHud_gen_CfgInitialize(&xv_hud,XV_Hud_cfg);

	XHud_gen_Set_row(&xv_hud, (u32) 480);
	XHud_gen_Set_column(&xv_hud, (u32) 640);
	XHud_gen_Set_ball_y(&xv_hud, (u32) (640/2));
	XHud_gen_Set_ball_x(&xv_hud, (u32) (280/2));
	XHud_gen_Set_ball_size(&xv_hud, (u32) 5);
	XHud_gen_Set_ball_colour(&xv_hud, (u32) 0x7ff0f0f0);
	XHud_gen_EnableAutoRestart(&xv_hud);
	XHud_gen_Start(&xv_hud);

	XVtc_Enable(&VtcInst);

    u32 height,width,status;

    tpg_config = XV_tpg_LookupConfig(XPAR_XV_TPG_0_DEVICE_ID);
    XV_tpg_CfgInitialize(&tpg, tpg_config, tpg_config->BaseAddress);


    status = XV_tpg_IsReady(&tpg);
	printf("TPG Status %u \n\r", (unsigned int) status);

	XV_tpg_Set_height(&tpg, (u32) video.height);
	XV_tpg_Set_width(&tpg, (u32) video.width);
	height = XV_tpg_Get_height(&tpg);
	width = XV_tpg_Get_width(&tpg);
	XV_tpg_Set_colorFormat(&tpg,XVIDC_CSF_RGB);
	XV_tpg_Set_maskId(&tpg, 0x0);
	XV_tpg_Set_motionSpeed(&tpg, 0x4);

	printf("info from tpg %u %u \n\r", (unsigned int)height, (unsigned int)width);
	XV_tpg_Set_bckgndId(&tpg,XTPG_BKGND_COLOR_BARS); //);

	status = XV_tpg_Get_bckgndId(&tpg);
	printf("Status %x \n\r", (unsigned int) status);
	XV_tpg_EnableAutoRestart(&tpg);
	XV_tpg_Start(&tpg);

	status = XV_tpg_IsIdle(&tpg);

	//  x=x+vx
	//  y=y+vy
	//  if x<0 or x>WIDTH:
	//    vx=-vx
	//    x=x+vx
	//  if y<0 or y>HEIGHT:
	//    vy=-vy
	//    y=y+vy

	printf("Status %u \n\r", (unsigned int) status);
	int x = 0;
	int vx = -7;
	int y = 0;
	int vy = 5;
	int direction;
	int position = 320;
	int bar_width = 30;
	int score = 0;
	XHud_gen_Set_bar_width(&xv_hud, (u32) bar_width);
	while (1){

	usleep(25000);


	direction = XGpio_DiscreteRead(&gpio, 1);
	//printf("direction %u \n\r", (unsigned int) direction);
	x = x + vx;
	y = y + vy;

	if (x < 0){
		vx = -vx;
		x = x + vx;
	} else {
			if ( (x > 470) & ( (y >= (position )) & ( y <= (position + bar_width)))){
				vx = -vx;
				x = x + vx;
				score ++;

			} else {
				if (x > 470){
				 score = 0;
				 x = rand() % 470;
				 vx = rand() % 10;
				 y = rand() % 400;
				 vy = rand() % 10;
				}
			}
	}
	if ((y <0 )| (y > 560)) {
		vy = - vy;
		y = y + vy;
	}
	XGpio_DiscreteWrite(&gpio, 2, (u32)score);
	XHud_gen_Set_ball_x(&xv_hud, (u32) y);
	XHud_gen_Set_ball_y(&xv_hud, (u32) x);

	if(direction == 0x2){
		position = position - 10;
		XHud_gen_Set_bar_pos_x(&xv_hud, (u32) position);
	}else{
		if (direction == 0x4){
			position = position + 10;
			XHud_gen_Set_bar_pos_x(&xv_hud, (u32) position);
		} else {
			if (direction == 0x8){
				bar_width = bar_width + 10;
					XHud_gen_Set_bar_width(&xv_hud, (u32) bar_width);

			} else {
				if (direction == 0x1){
					bar_width = bar_width - 10;
					XHud_gen_Set_bar_width(&xv_hud, (u32) bar_width);
			}
			}
		}
	}



}
    cleanup_platform();
    return 0;
}
