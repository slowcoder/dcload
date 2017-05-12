#include "hw_config.h"

void USB_Interrupts_Config(void) {
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  /* 2 bit for pre-emption priority, 2 bits for subpriority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure);
}

static void IntToHex(uint32_t value , uint8_t *pbuf , uint8_t len);
extern uint8_t *Virtual_Com_Port_StringSerial;

void Get_USB_SerialNum(void) {
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = 0xCAFEBABE;
  Device_Serial1 = 0xDEADBEEF;
  Device_Serial2 = 0xF00D1E00;  

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToHex(Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToHex(Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

static void IntToHex(uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}

void USB_Cable_Config (FunctionalState NewState) {

}