#include <stdbool.h>
#include <ucos_ii.h>
#include <mbed.h>
#include "C12832.h"

/*
*********************************************************************************************************
*                                            APPLICATION TASK PRIORITIES
*********************************************************************************************************
*/

typedef enum {
  LED_RED_PRIO = 4,
  LED_GREEN_PRIO,
  APP_TASK_COUNT1_PRIO,
  APP_TASK_COUNT2_PRIO
} taskPriorities_t;

/*
*********************************************************************************************************
*                                            APPLICATION TASK STACKS
*********************************************************************************************************
*/

#define  LED_RED_STK_SIZE              256
#define  LED_GREEN_STK_SIZE            256
#define  APP_TASK_COUNT1_STK_SIZE      256
#define  APP_TASK_COUNT2_STK_SIZE      256

static OS_STK ledRedStk[LED_RED_STK_SIZE];
static OS_STK ledGreenStk[LED_GREEN_STK_SIZE];
static OS_STK appTaskCOUNT1Stk[APP_TASK_COUNT1_STK_SIZE];
static OS_STK appTaskCOUNT2Stk[APP_TASK_COUNT2_STK_SIZE];

/*
*********************************************************************************************************
*                                            APPLICATION FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void appTaskLedRed(void *pdata);
static void appTaskLedGreen(void *pdata);
static void appTaskCOUNT1(void *pdata);
static void appTaskCOUNT2(void *pdata);

static void display(uint8_t id, uint32_t value);
static void ledToggle(DigitalOut led);
/*
*********************************************************************************************************
*                                            GLOBAL TYPES AND VARIABLES 
*********************************************************************************************************
*/
static C12832 lcd(D11, D13, D12, D7, D10);
static bool flashing = false;
static uint32_t total = 0;
static uint32_t count1 = 0;
static uint32_t count2 = 0;

static OS_EVENT *lcdSem1;
static OS_EVENT *lcdSem2;
/*
*********************************************************************************************************
*                                            GLOBAL FUNCTION DEFINITIONS
*********************************************************************************************************
*/

int main() {

  /* Initialise the display */
  lcd.cls();

  /* Initialise the OS */
  OSInit();                                                   

  /* Create the tasks */
  OSTaskCreate(appTaskLedRed,                               
               (void *)0,
               (OS_STK *)&ledRedStk[LED_RED_STK_SIZE - 1],
               LED_RED_PRIO);
  
  OSTaskCreate(appTaskLedGreen,                               
               (void *)0,
               (OS_STK *)&ledGreenStk[LED_GREEN_STK_SIZE - 1],
               LED_GREEN_PRIO);

  OSTaskCreate(appTaskCOUNT1,                               
               (void *)0,
               (OS_STK *)&appTaskCOUNT1Stk[APP_TASK_COUNT1_STK_SIZE - 1],
               APP_TASK_COUNT1_PRIO);

  OSTaskCreate(appTaskCOUNT2,                               
               (void *)0,
               (OS_STK *)&appTaskCOUNT2Stk[APP_TASK_COUNT2_STK_SIZE - 1],
               APP_TASK_COUNT2_PRIO);

  /* Create the semaphore */
  lcdSem1 = OSSemCreate(1);
  lcdSem2 = OSSemCreate(0);

  
  /* Start the OS */
  OSStart();                                                  
  
  /* Should never arrive here */ 
  return 0;      
}

/*
*********************************************************************************************************
*                                            APPLICATION TASK DEFINITIONS
*********************************************************************************************************
*/

static void appTaskLedRed(void *pdata) {
  DigitalOut red(LED_RED);
  
  /* Start the OS ticker -- must be done in the highest priority task */
  SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
  
  red = 1;

  /* Task main loop */
  while (true) {
    if (flashing) {
      ledToggle(red);
    }
    OSTimeDlyHMSM(0,0,0,500);
  }
}

static void appTaskLedGreen(void *pdata) {
  DigitalOut green(LED_GREEN);

  green = 0;
  while (true) {
    if (flashing) {
      ledToggle(green);
    }
    OSTimeDlyHMSM(0,0,0,500);
  } 
}

static void appTaskCOUNT1(void *pdata) {  
  uint8_t status;
	
  //OSTimeDlyHMSM(0,0,0,1);
  while (true) {
    OSSemPend(lcdSem1, 0, &status);
    count1 += 1;
    display(1, count1);
    total += 1;
    status = OSSemPost(lcdSem2);
    if ((count1 + count2) != total) {
      flashing = true;
    }
    OSTimeDlyHMSM(0,0,0,2);
  } 
}

static void appTaskCOUNT2(void *pdata) {
  uint8_t status;
	
  while (true) {
    OSSemPend(lcdSem2, 0, &status);
    count2 += 1;
    display(2, count2);
    total += 1;
    status = OSSemPost(lcdSem1);		
    if ((count1 + count2) != total) {
      flashing = true;
    }
    OSTimeDlyHMSM(0,0,0,2);
  } 
}

static void ledToggle(DigitalOut led) {
  led = 1 - led;
}

static void display(uint8_t id, uint32_t value) {
    lcd.locate(0, id * 8);
    lcd.printf("count%1d: %09d", id, value);
}
