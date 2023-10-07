/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "lwip.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "usbd_cdc_if.h"
#include "eeprma2_m24.h"

#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Use USB Device CDC instead of USART1 for debug output */
/* #define USB_DEBUG */

/* Enable the USB Host stack (USB Disk) */
/* #define ENABLE_USBHOST */

/* Enable the LWIP Ethernet Stack */
/* #define ENABLE_ETHERNET */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

void MX_EEPRMA2_Check_24C02(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_RTC_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
#ifdef ENABLE_ETHERNET
  MX_LWIP_Init();
#endif
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();
  MX_CRC_Init();
#ifdef ENABLE_USBHOST
  MX_USB_HOST_Init();
#endif
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  printf("\r\nInit Complete.\r\n");
  printf("Checking Storage Devices:\r\n");
  MX_EEPRMA2_Check_24C02();
  MX_SPI2_Check_W25Q64();
  MX_SDIO_SD_Check();

  printf("Checking CAN Devices:\r\n");
  MX_CAN_Loopback_Check();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifdef ENABLE_ETHERNET
    MX_LWIP_Process();
#endif
    /* USER CODE END WHILE */

#ifdef ENABLE_USBHOST
    MX_USB_HOST_Process();
#endif
    /* USER CODE BEGIN 3 */
  }

  /* Something went wrong */
  NVIC_SystemReset();
  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief Override for printf output
  * @param  file: pointer to the source file name
  * @param  ptr: pointer to data to write
  * @param  len: length data to write in bytes
  * @retval Number of bytes written
  */
int _write(int file, char *ptr, int len)
{
    HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_RESET);

#ifdef USB_DEBUG
    static uint8_t rc = USBD_OK;
    bool wait = false;

    /* Wait for terminal to be opened */
    while (!CDC_Is_Connected())
    {
      wait = true;
    }

    /* If the terminal has just opened, give it some extra time */
    if (wait)
    {
      HAL_Delay(250);
    }

    /* Send the data, retrying if busy */
    do {
        rc = CDC_Transmit_FS((uint8_t*)ptr, len);
    } while (USBD_BUSY == rc);

    if (USBD_FAIL == rc) {
        /// NOTE: Should never reach here.
        /// TODO: Handle this error.
        return 0;
    }
#else
    HAL_StatusTypeDef rc;
    do {
      /* Send the data, retrying if busy */
      rc = HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 100);
    } while (rc == HAL_BUSY);
#endif

    HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_SET);

    return len;
}

/**
  * @brief Check for presence of I2C Flash
  * @retval None
  */
void MX_EEPRMA2_Check_24C02(void)
{
  int32_t ret = EEPRMA2_M24_Init(EEPRMA2_M24C02_0);
  if (ret == BSP_ERROR_NONE)
  {
    ret = EEPRMA2_M24_IsDeviceReady(EEPRMA2_M24C02_0, 10);
    if (ret == BSP_ERROR_NONE)
    {
      printf("I2C 24C02:    256B\r\n");
    }
    else
    {
      printf("I2C 24C02:    Error\r\n");
    }
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
