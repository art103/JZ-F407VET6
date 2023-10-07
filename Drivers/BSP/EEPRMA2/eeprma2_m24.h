/**
 ******************************************************************************
  * @file    eeprma2_m24.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header of eeprma expansion board based on m24xx series eeprom
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPRMA2_M24_H
#define __EEPRMA2_M24_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "m24xx.h" 
#include "eeprma2_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup X_NUCLEO_EEPRMA2
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/**
 * @brief  EEPROM enumerator definition
 */
  
/**

  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup EEPRMA2_M24_Exported_Constants   EEPRMA M24 Exported Constants
  * @{
  */

#define EEPRMA2_M24_INSTANCES_NBR		3U 
#define EEPRMA2_M24C02_0                         0U
#define EEPRMA2_M24256_0                         1U
#define EEPRMA2_M24M01_0                         2U  
#define EEPRMA2_M24xx                            7U



/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup EEPRMA2_M24_Exported_Functions   EEPRMA M24 Exported Functions
  * @{
  */

int32_t EEPRMA2_M24_Init(uint32_t Instance);
int32_t EEPRMA2_M24_DeInit(uint32_t Instance);
int32_t EEPRMA2_M24_IsDeviceReady( uint32_t Instance, const uint32_t Trials );
int32_t EEPRMA2_M24_ReadByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEPRMA2_M24_ReadData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M24_ReadPage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M24_WriteByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr);
int32_t EEPRMA2_M24_WriteData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M24_WritePage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M24_WriteID(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA2_M24_ReadID(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA2_M24_LockStatus( uint32_t Instance, uint8_t * pData);
int32_t EEPRMA2_M24_LockID( uint32_t Instance);
void EEPRMA2_M24MemorySizeLocator(uint32_t Instance);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
  }
#endif
#endif /* __X_NUCLEO_EEPRMA_H */

