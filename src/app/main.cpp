#include <stdbool.h>
#include <ucos_ii.h>
#include <mbed.h>

/*
*********************************************************************************************************
*                                            APPLICATION TASK PRIORITIES
*********************************************************************************************************
*/

typedef enum {
  LED_RED_PRIO = 4,
  LED_GREEN_PRIO
} taskPriorities_t;

/*
*********************************************************************************************************
*                                            APPLICATION TASK STACKS
*********************************************************************************************************
*/

#define  LED_RED_STK_SIZE              256
#define  LED_GREEN_STK_SIZE            256

static OS_STK ledRedStk[LED_RED_STK_SIZE];
static OS_STK ledGreenStk[LED_GREEN_STK_SIZE];

/*
*********************************************************************************************************
*                                            APPLICATION FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void appTaskLedRed(void *pdata);
static void appTaskLedGreen(void *pdata);

static void ledToggle(DigitalOut led);
/*
*********************************************************************************************************
*                                            GLOBAL TYPES AND VARIABLES 
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                            GLOBAL FUNCTION DEFINITIONS
*********************************************************************************************************
*/

int main() {

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
    ledToggle(red);
    OSTimeDlyHMSM(0,0,0,500);
  }
}

static void appTaskLedGreen(void *pdata) {
  DigitalOut green(LED_GREEN);

  green = 0;
  while (true) {
    ledToggle(green);	
    OSTimeDlyHMSM(0,0,0,500);
  } 
}

static void ledToggle(DigitalOut led) {
  led = 1 - led;
}
