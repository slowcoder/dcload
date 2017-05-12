#include "stm32f10x.h"

// DAC0 = PA4 - Iset
// DAC1 = PA5 - Not used

static DAC_InitTypeDef DAC_InitStructure;

int dac_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_Cmd(DAC_Channel_1, ENABLE);

    /* Set DAC Channel1 DHR12L register */
    /*  0 = 0mA Iset */
    DAC_SetChannel1Data(DAC_Align_12b_L, 0x0000);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);

    return 0;
}

int dac_setOutput(uint16_t uOutValue) {
    uOutValue &= 0xFFF0;

    DAC_SetChannel1Data(DAC_Align_12b_L, uOutValue);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);

    return 0;
}
