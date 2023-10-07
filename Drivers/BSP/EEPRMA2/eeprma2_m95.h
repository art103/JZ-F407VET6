/**
  ******************************************************************************
  * @file    eeprma2_m95.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header of eeprma expansion board based on m95xx series eeprom
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
#ifndef __EEPRMA2_M95_H
#define __EEPRMA2_M95_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "m95xx.h"   
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

#define EEPRMA2_M95_INSTANCES_NBR                3U 
#define EEPRMA2_M95M04_0                         0U
#define EEPRMA2_M95256_0                         1U
#define EEPRMA2_M95040_0                         2U
#define EEPRMA2_M95xx                            8U

/**
 * @brief  M95xx Memory information structure definition
 */

/**
 * @brief  EEPROM enumerator definition
 */
  
typedef enum 
{
  SS1 = 0,
  SS2,
  SS3,
  SS4,
  WC, 
  HOLD,
  WCTRL  
} EEPROMEX_CTRL_PIN;


/**
 * @brief  EEPROM structure definition
 */
typedef struct{
  uint16_t          EEPROMEX_PIN;
  GPIO_TypeDef *    EEPROMEX_PIN_PORT;
}EEPROMEX_CtrlPin_TypeDef;


#define EEPROMEX_WC_PIN                 GPIO_PIN_1
#define EEPROMEX_WC_PIN_PORT            GPIOA


#define EEPROMEX_SLAVE_FOUR_PIN          GPIO_PIN_0
#define EEPROMEX_SLAVE_FOUR_PIN_PORT     GPIOC


#define EEPROMEX_SPI_SLAVE_FOUR_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
/* External variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup EEPRMA2_M24_Exported_Functions   EEPRMA M24 Exported Functions
  * @{
  */

int32_t EEPRMA2_M95_Init(uint32_t Instance);
int32_t EEPRMA2_M95_DeInit(uint32_t Instance);
int32_t EEPRMA2_M95_WriteReg(uint32_t Instance, uint8_t pData);
int32_t EEPRMA2_M95_ReadReg(uint32_t Instance, uint8_t * pData);
int32_t EEPRMA2_M95_ReadByte(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr);
int32_t EEPRMA2_M95_ReadPage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M95_ReadData(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr,  const uint16_t Size);
int32_t EEPRMA2_M95_WriteByte(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr);
int32_t EEPRMA2_M95_WritePage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size);
int32_t EEPRMA2_M95_WriteData(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr,  const uint16_t Size);
int32_t EEPRMA2_M95_WriteID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA2_M95_ReadID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size );
int32_t EEPRMA2_M95_LockStatus( uint32_t Instance, uint8_t * pData);
int32_t EEPRMA2_M95_LockID( uint32_t Instance);
int32_t EEPRMA2_SPI_ReadReg(uint8_t * pData, uint8_t Devaddr);
int32_t EEPRMA2_SPI_RecvBuffer(uint8_t * pData, uint32_t TarAddr, 
                                        uint32_t DevAddr, const uint16_t Size, const uint8_t Inst );
int32_t EEPRMA2_SPI_SendBuffer(uint8_t * pData,uint32_t DevAddr, 
                                    uint32_t TarAddr,const uint16_t Size, const uint8_t Inst);
int32_t EEPRMA2_SPI_WriteReg(uint8_t pData, uint32_t Devaddr);
int32_t EEPRMA2_M95_IOWrite(const uint8_t TxData );
int32_t EEPRMA2_M95_WriteCmd(uint8_t Cmd,const uint8_t Devaddr);
int32_t EEPRMA2_M95_IsDeviceReady(uint8_t Devaddr);
int32_t EEPRMA2_M95_WriteAddr( const uint32_t TarAddr, const uint32_t DevAddr);
void EEPRMA2_M95MemorySizeLocator(uint32_t Instance);
/**
  * @}
  */
void EEPROMEX_CTRL_HIGH(const uint8_t pin);
void EEPROMEX_CTRL_LOW(const uint8_t pin);
void EEPRMA2_GPIO_Init( void );
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

