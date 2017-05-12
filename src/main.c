#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "uart.h"
#include "buttons.h"
#include "adc.h"
#include "dac.h"
#include "adc.h"
#include "cli/cli.h"
#include "log.h"

static osThreadId defaultTaskHandle;

static struct {
    int bOn;
    uint16_t uIset;
} state;

void StartDefaultTask(void const * argument) {
    uint32_t tmp;

    LOG("");
    LOG("System starting");

    tmp = RCC->CFGR & RCC_CFGR_SWS;
    if( tmp == 0x00 ) LOG("System clock source: HSI");
    else if( tmp == 0x04 ) LOG("System clock source: HSE");
    else if( tmp == 0x08 ) LOG("System clock source: PLL");
    else LOG("System clock source: HSIx");

    LOG(" * Buttons init");
    buttons_init();
    LOG(" * DAC init");
    dac_init();
    LOG(" * ADC init");
    adc_init();
    LOG(" * CLI init");
    cli_init();

    LOG(" * USB Interrupt init");
    USB_Interrupts_Config();
    LOG(" * USB core init");
    USB_Init();

    dac_setOutput(0x10);

    int iDAC = 0x0;
    for(;;) {
        uint16_t adc;

  //      dac_setOutput(iDAC);
        osDelay(100);
        adc = adc_getValue();
//        LOG("DAC=0x%x, ADC=%u",iDAC,adc);
        

        osDelay(1000);
        iDAC += 0x100;
        if(iDAC >= 0x2000) iDAC = 0x0;
    }

    for(;;) {
#if 1
        eButton btn;

        do {
            btn = buttons_poll();
            if( btn == eButton_BTN0 ) {
                state.bOn = !state.bOn;
                LOG("State: %i",state.bOn);
            }
        } while( btn != eButton_Invalid );
#endif
        osDelay(1);
    }
}

int main(void) {
    SystemCoreClockUpdate();

    uart_init();

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 2048);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    osKernelStart();

    for(;;) {
    }
    return 0;
}

