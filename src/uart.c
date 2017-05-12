#include <string.h>

#include "stm32f10x.h"
#include "uart.h"

static void RCC_Configuration(void);
static void GPIO_Configuration(void);

USART_InitTypeDef USART_InitStructure;

#define RX_INTERRUPT

int  uart_init(void) {
    RCC_Configuration();
    GPIO_Configuration();

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);


#ifdef RX_INTERRUPT
    NVIC_InitTypeDef NVIC_InitStructure;

    memset(&NVIC_InitStructure,0,sizeof(NVIC_InitTypeDef));

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(USART1_IRQn,5);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif


    return 0;
}

void upper_UART_ISR(int i);
#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)

void USART1_IRQHandler(void) {
    int i;

    if( (USART1->SR & USART_FLAG_RXNE) != (u16)RESET ) {          
        i = USART_ReceiveData(USART1);

        USART_ClearFlag(USART2,USART_FLAG_RXNE);

        upper_UART_ISR(i);
    }

    uint32_t status;
    status = USART1->SR;
    while(status & (USART_FLAG_RXNE | USART_FLAG_ERRORS)) {
        uint8_t ch;

        ch = USART1->DR;    // Read Data, or Clear Error(s)
        ch++;
        status = USART1->SR;
    }
}

static void sendChar(const char ch) {
    /* Send one byte from USART1 */
    USART_SendData(USART1, ch);

    /* Loop until USARTy DR register is empty */ 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
    }
}

void uart_puts(const char *pzStr) {
    char *p;

    p = (char*)pzStr;
    while( *p != 0 ) {
        sendChar(*p);
        p++;
    }
}



static void RCC_Configuration(void) {
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | 
                            RCC_APB2Periph_AFIO | 
                            RCC_APB2Periph_GPIOA, ENABLE);
}

static void GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}