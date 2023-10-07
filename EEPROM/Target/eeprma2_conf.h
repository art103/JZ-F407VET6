
/**
  ******************************************************************************
  * @file           : eeprma2_conf.h
  * @author  		: SRA Application Team
  * @brief   		: This file contains definitions for the EEPRMA2 components bus interfaces
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
#ifndef __EEPRMA2_CONF_H__
#define __EEPRMA2_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "custom_bus.h"
#include "custom_errno.h"

/* Global configuration ---------------------------------------------------*/
#define M24256_I2C_ADDR    0xAAU
#define M24M01_I2C_ADDR    0xACU
#define M24C02_I2C_ADDR    0xA0U	// This is different to the MX Cube Default
#define M95M04_SPI_ADDR    0xCCU
#define M95256_SPI_ADDR    0xC9U
#define M95040_SPI_ADDR    0xC6U

/******************************************************************************/

/* Defines ---------------------------------------------------------------*/

#define EEPRMA2_I2C_Init         		BSP_I2C1_Init
#define EEPRMA2_I2C_DeInit       		BSP_I2C1_DeInit
#define EEPRMA2_I2C_ReadReg         	BSP_I2C1_ReadReg
#define EEPRMA2_I2C_WriteReg         	BSP_I2C1_WriteReg
#define EEPRMA2_I2C_ReadReg16     		BSP_I2C1_ReadReg16
#define EEPRMA2_I2C_WriteReg16			BSP_I2C1_WriteReg16
#define EEPRMA2_I2C_IsReady             BSP_I2C1_IsReady
#define EEPRMA2_I2C_Send				BSP_I2C1_Send
#define EEPRMA2_M24_Delay          	    HAL_Delay

#ifdef __cplusplus
}
#endif

#endif /* __EEPRMA2_CONF_H__*/

