/*
 * mainLoop.c
 *
 *  Created on: 22. juni 2015
 *      Author: gist
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_rtc.h"
#include "em_lcd.h"
#include "bsp.h"


#include "mainLoop.h"

//#include "PID.h"
//#include "readTemp.h"
//#include "led.h"
#include "timer.h"
#include "slider.h"
#include "menuAlt.h"


uint32_t msTicks = 0;

//Statusflag for scheduler
bool flag_LED = false;
bool flag_PID = false;
bool flag_MenuUpdate = false;
bool flag_TimerTick = false;

//Periods for scheduler

#define MENU_PER 50
#define LEDTOGGLE_PER 1000
#define TIMERTICK_PER 3000
#define PID_PER 2000

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PB1)
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
   //Menu_Select(); //Event
  event_buttonSelect();

  /* Acknowledge interrupt */
  GPIO_IntClear(1 << 11);
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PB0)
 *****************************************************************************/
void GPIO_EVEN_IRQHandler(void){

  //Menu_Back();  //Event
  event_buttonBack();

   /* Acknowledge interrupt */
  GPIO_IntClear(1 << 8);
}

/**************************************************************************//**
 * @ 1 ms system tick handler
 *****************************************************************************/
void SysTick_Handler(void){
  msTicks++;       /* increment counter necessary in Delay()*/

  if(msTicks%LEDTOGGLE_PER == 0){
    flag_LED = 1;
  }
  if(msTicks%MENU_PER == 0){
    flag_MenuUpdate = 1;
  }
  if(msTicks%TIMERTICK_PER == 0){
    flag_TimerTick = 1;
  }
  if(msTicks%PID_PER == 0){
    flag_PID = 1;
  }
}

void main_loop(void){


	  CMU_ClockEnable(cmuClock_HFPER, true);


	  /* Setup SysTick Timer for 1 msec interrupts  */
	  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

	  //Setup LED
	  //LED_Init();

	  //Setup PID
	  //PID_Init();

	  /* Initialize MENU */
	  MENU_Init(); //new version

	  //Setup touch slider
	  SLIDER_Init();

	//Main loop, checks for button/slider events and status flags.
	int32_t tmp,tmp2;
	while (1)
	{
	  tmp = SLIDER_posDiff();
	  if (tmp != 0){
		  event_sliderDiff(tmp);
	  }

	  tmp2 = SLIDER_posChange();
	  if (tmp2 != 0){
		  event_sliderChange(tmp2);
	  }

	  //Mini scheduler
	  if(flag_LED){
		//LED_Toggle();
		flag_LED = false;
	  }
	  if(flag_PID){
		//PID_runController();
		flag_PID = false;
	  }
	  if(flag_MenuUpdate){
		MENU_update();
		flag_MenuUpdate = false;
	  }
	  if(flag_TimerTick){
		TIMER_tick(TIMERTICK_PER);
		flag_TimerTick = false;
	  }
	}
}

