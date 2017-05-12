#include "stm32f10x.h"

//ADC0 = PA0 - I-Sense
//ADC1 = PA1 - Not used

int adc_init(void) {
    ADC_InitTypeDef conf;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    conf.ADC_Mode = ADC_Mode_Independent;
    conf.ADC_ScanConvMode = ENABLE;
    conf.ADC_ContinuousConvMode = ENABLE;
    conf.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    conf.ADC_DataAlign = ADC_DataAlign_Right;
    conf.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&conf);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1, ENABLE);

    // Calibrate the ADC
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    return 0;
}

uint16_t adc_getValue(void) {
#if 0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    return ADC_GetConversionValue(ADC1);
#else
    int iValue;

    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    iValue = ADC_GetConversionValue(ADC1);

    return iValue & 0xFFFF;
#endif
}
