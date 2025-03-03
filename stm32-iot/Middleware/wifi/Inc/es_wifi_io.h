/**
  ******************************************************************************
  * @file    es_wifi_io.h
  * @author  MCD Application Team
  * @brief   This file contains the functions prototypes for es_wifi IO operations.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef WIFI_IO_H
#define WIFI_IO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */ 

void SPI_WIFI_attach(SPI_HandleTypeDef* xhspi,
 		GPIO_TypeDef* reset_gpio,
 		uint16_t reset_pin,
 		GPIO_TypeDef* csn_gpio,
 		uint16_t csn_pin,
 		GPIO_TypeDef* ready_gpio,
 		uint16_t ready_pin);

int8_t  SPI_WIFI_DeInit(void);
int8_t  SPI_WIFI_Init(uint16_t mode);
int8_t  SPI_WIFI_ResetModule(void);
int16_t SPI_WIFI_ReceiveData(uint8_t *pData, uint16_t len, uint32_t timeout);
int16_t SPI_WIFI_SendData( uint8_t *pData, uint16_t len, uint32_t timeout);
void    SPI_WIFI_Delay(uint32_t Delay);
void    SPI_WIFI_ISR(void);

uint8_t WIFI_IS_CMDDATA_READY(void);
void WIFI_DISABLE_NSS(void);
void WIFI_ENABLE_NSS(void);
void WIFI_RESET_MODULE(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_IO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
