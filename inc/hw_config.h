#pragma once

#include <stdint.h>

#include "stm32f10x.h"
#include "usb_type.h"

void USB_Interrupts_Config(void);

void USB_To_USART_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes);
void Handle_USBAsynchXfer (void);
//void USART_To_USB_Send_Data(void);

void Get_USB_SerialNum(void);
