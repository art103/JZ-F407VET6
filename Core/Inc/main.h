/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "eeprma2_conf.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DS18B20_Pin GPIO_PIN_2
#define DS18B20_GPIO_Port GPIOE
#define SPI2_FLASH_CS_Pin GPIO_PIN_3
#define SPI2_FLASH_CS_GPIO_Port GPIOE
#define P4_GPIO_Pin GPIO_PIN_4
#define P4_GPIO_GPIO_Port GPIOE
#define P4_GPIOE5_Pin GPIO_PIN_5
#define P4_GPIOE5_GPIO_Port GPIOE
#define P4_GPIOE6_Pin GPIO_PIN_6
#define P4_GPIOE6_GPIO_Port GPIOE
#define P4_GPIOC13_Pin GPIO_PIN_13
#define P4_GPIOC13_GPIO_Port GPIOC
#define P4_GPIOC0_Pin GPIO_PIN_0
#define P4_GPIOC0_GPIO_Port GPIOC
#define P4_GPIOA0_Pin GPIO_PIN_0
#define P4_GPIOA0_GPIO_Port GPIOA
#define P4_GPIOA3_Pin GPIO_PIN_3
#define P4_GPIOA3_GPIO_Port GPIOA
#define P4_GPIOA4_Pin GPIO_PIN_4
#define P4_GPIOA4_GPIO_Port GPIOA
#define P4_GPIOA5_Pin GPIO_PIN_5
#define P4_GPIOA5_GPIO_Port GPIOA
#define P4_GPIOA6_Pin GPIO_PIN_6
#define P4_GPIOA6_GPIO_Port GPIOA
#define P5_GPIO_Pin GPIO_PIN_0
#define P5_GPIO_GPIO_Port GPIOB
#define P5_GPIOB1_Pin GPIO_PIN_1
#define P5_GPIOB1_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define NRF_CE_Pin GPIO_PIN_7
#define NRF_CE_GPIO_Port GPIOE
#define SPI2_NRF_CS_Pin GPIO_PIN_8
#define SPI2_NRF_CS_GPIO_Port GPIOE
#define NRF_IRQ_Pin GPIO_PIN_9
#define NRF_IRQ_GPIO_Port GPIOE
#define SW1_Pin GPIO_PIN_10
#define SW1_GPIO_Port GPIOE
#define SW2_Pin GPIO_PIN_11
#define SW2_GPIO_Port GPIOE
#define SW3_Pin GPIO_PIN_12
#define SW3_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOE
#define P5_GPIOD8_Pin GPIO_PIN_8
#define P5_GPIOD8_GPIO_Port GPIOD
#define P5_GPIOD9_Pin GPIO_PIN_9
#define P5_GPIOD9_GPIO_Port GPIOD
#define P5_GPIOD10_Pin GPIO_PIN_10
#define P5_GPIOD10_GPIO_Port GPIOD
#define P5_GPIOD11_Pin GPIO_PIN_11
#define P5_GPIOD11_GPIO_Port GPIOD
#define P5_GPIOD12_Pin GPIO_PIN_12
#define P5_GPIOD12_GPIO_Port GPIOD
#define P5_GPIOD13_Pin GPIO_PIN_13
#define P5_GPIOD13_GPIO_Port GPIOD
#define P5_GPIOD14_Pin GPIO_PIN_14
#define P5_GPIOD14_GPIO_Port GPIOD
#define P5_GPIOD15_Pin GPIO_PIN_15
#define P5_GPIOD15_GPIO_Port GPIOD
#define P5_GPIOC6_Pin GPIO_PIN_6
#define P5_GPIOC6_GPIO_Port GPIOC
#define P5_GPIOC7_Pin GPIO_PIN_7
#define P5_GPIOC7_GPIO_Port GPIOC
#define P5_GPIOA8_Pin GPIO_PIN_8
#define P5_GPIOA8_GPIO_Port GPIOA
#define SDIO_CD_Pin GPIO_PIN_3
#define SDIO_CD_GPIO_Port GPIOD
#define P4_GPIOD4_Pin GPIO_PIN_4
#define P4_GPIOD4_GPIO_Port GPIOD
#define RS485_TX_RX__Pin GPIO_PIN_7
#define RS485_TX_RX__GPIO_Port GPIOD
#define P4_GPIOB7_Pin GPIO_PIN_7
#define P4_GPIOB7_GPIO_Port GPIOB
#define P4_GPIOE0_Pin GPIO_PIN_0
#define P4_GPIOE0_GPIO_Port GPIOE
#define P4_GPIOE1_Pin GPIO_PIN_1
#define P4_GPIOE1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
