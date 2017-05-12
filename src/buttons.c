#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"

#include "stm32f10x.h"
#include "log.h"

#include "buttons.h"

/*
 All buttons should be configured to pull-down,
 as they're pulled up when actuated 
BTN0 (PB) = PA6  - EXTI6
BTN1 (PB) = PA7  - EXTI7
BTN2 (RS) = PC13 - EXTI13
BTN3 (RB) = PC15 - EXTI14
BTN4 (RA) = PC14 - EXTI15

EXTI9_5_IRQHandler
EXTI15_10_IRQHandler
*/

static QueueHandle_t xButtonQueue;

volatile uint32_t uNumIRQs = 0;

typedef struct {
    eButton btn;
} btn_t;

int     buttons_init(void) {
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    xButtonQueue = xQueueCreate( 20, sizeof(btn_t) );
    if( xButtonQueue == NULL ) {
        LOGE("Failed to create button queue");
        return -1;
    }

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // GPIO Config
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // EXTI Config
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);

    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_Init(&EXTI_InitStructure);

    // NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//    NVIC_Init(&NVIC_InitStructure);

    return 0;
}

eButton buttons_poll(void) {
    btn_t btn;

    if( xQueueReceive(xButtonQueue,&btn,1) == pdTRUE ) {
        return btn.btn;
    }

    return eButton_Invalid;
}

void EXTI9_5_IRQHandler(void) {
    btn_t btn;
    uint16_t bits;

    uNumIRQs++;

    btn.btn = eButton_Invalid;

    bits = GPIO_ReadInputData(GPIOA);

    if( bits & (1<<6) ) btn.btn = eButton_BTN0;
    if( bits & (1<<7) ) btn.btn = eButton_BTN1;

    if( btn.btn != eButton_Invalid )
        xQueueSendToBackFromISR( xButtonQueue, &btn, NULL );

    EXTI_ClearITPendingBit(EXTI_Line6);
    EXTI_ClearITPendingBit(EXTI_Line7);
}

void EXTI15_10_IRQHandler(void) {
    uNumIRQs++;

    EXTI_ClearITPendingBit(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line15);
}
