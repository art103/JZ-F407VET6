/**
  ******************************************************************************
  * @file    m24xx.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   header file of m24xx based eeprom driver 
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
#ifndef __M24_XX_H
#define __M24_XX_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
/** @addtogroup M24
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup M24_Exported_Types
  * @{
  */



/* Exported constants --------------------------------------------------------*/
/** @defgroup M24_Exported_Constants
  * @{
  */
#ifndef NULL
#define NULL      (void *) 0
#endif
#define MIN_TRIALS              2
#define LOCKID                  0x02   /* Byte to be send to lock ID page*/    
#define ADDRLID_16              0x0400
  
#define ADDRLID_SPI             0x0480    
#define LOCKDATA_SPI            0x02
    
#define BITCOUNT                0x01  /* Initial count value */
#define IDMASK                  0x04  /* Mask fith bit of I2C mem device address */
    
#define EEPROMEX_WREN           0x06  /*!< Write enable instruction */
#define EEPROMEX_WRDI           0x04  /*!< Write disable instruction */
#define EEPROMEX_RDSR           0x05  /*!< Read Status Register instruction  */
#define EEPROMEX_WRSR           0x01  /*!< Write Status Register instruction */
#define EEPROMEX_WRITE          0x02  /*!< Lower Half Write to Memory instruction */
#define EEPROMEX_UPWRITE        0x0A  /*!< Upper Half Write to Memory instruction */
#define EEPROMEX_READ           0x03  /*!< Lower Half Read from Memory instruction */
#define EEPROMEX_UPREAD         0x0B  /*!< Upper Half Read from Memory instruction */
#define EEPROMEX_RDID           0x83  /*!< Read identifiction instruction */
#define EEPROMEX_WRID           0x82  /*!< Write identifiction instruction */
#define EEPROMEX_RDLS           0x83  /*!< Read ID page lock status instruction */
#define EEPROMEX_LID            0x82  /*!< Lock ID page in read only mode instruction */
#define EEPROMEX_FullProtect    0x0C  /*!< Whole memory protect from write - Write BP1 BP2 bits as 11 in status register */
#define EEPROMEX_UHalfProtect   0x08  /*!< Upper Half Protect from write - Write BP1 BP2 bits as 10 in status register */
#define EEPROMEX_UQuarterProtect 0x04  /*!< Upper Quarter Protect from write - Write BP1 BP2 bits as 01 in status register */
#define EEPROMEX_UnProtect      0x00  /*!< Un Protect from read only mode - Write BP1 BP2 bits as 10 in status register */

/** @defgroup M24XX error codes
  * @}
  */

#define M24_OK              0
#define M24_ERROR   	   -1
#define M24_BUSY    	   -2
#define M24_TIMEOUT 	   -3
#define M24_ADDR_OVERFLOW  -4

/* External variables --------------------------------------------------------*/

  extern uint16_t DeviceAddr;
  extern uint16_t PageSize;

 /*!
 * @brief Initializes the I2C  object 
 * @param  none
 * @retval Error code: 0 ok, negative KO
 */
typedef int32_t     (*M24_Init_Func)(void);   
typedef int32_t     (*M24_DeInit_Func)(void); 
typedef int32_t     (*M24_WriteReg_Func)( uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24_WriteReg16_Func)( uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24_Transmit_Func)( uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24_ReadReg16_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t     (*M24_IsReady_Func) (uint16_t, const uint32_t);
typedef void        (*M24_Delay)(uint32_t );   

typedef struct
{
  M24_Init_Func            Init;
  M24_DeInit_Func          DeInit;
  M24_WriteReg_Func        WriteReg;
  M24_WriteReg16_Func      WriteReg16;
  M24_Transmit_Func        Transmit;
  M24_ReadReg_Func	   ReadReg;
  M24_ReadReg16_Func	   ReadReg16;
  M24_IsReady_Func         IsReady;
  M24_Delay                Delay;
  uint8_t                  Address;
} M24_IO_t;

typedef struct
{
  M24_IO_t        IO;
} M24_Object_t;

/**
 * @brief  EEPROMEX COMMON driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M24_Object_t * );
  int32_t       (*DeInit)( M24_Object_t * );
  int32_t       (*IsReady)(M24_Object_t *, const uint32_t );
  int32_t       (*ReadRegister)( M24_Object_t *, uint8_t * const, const uint8_t );
  int32_t       (*WriteRegister)(M24_Object_t *, const uint8_t, const uint8_t );  
  int32_t       (*ReadByte)( M24_Object_t *, uint8_t *, const uint32_t);
  int32_t       (*WriteByte)(M24_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M24_Object_t *, uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M24_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t ); 
  int32_t       (*ReadData)( M24_Object_t *, uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M24_Object_t *, uint8_t *, const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteID)(M24_Object_t *,  uint8_t *, const uint32_t ,const uint16_t , const uint16_t);
  int32_t       (*ReadID)(M24_Object_t *, uint8_t * , const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*LockID)(M24_Object_t *); 
  int32_t       (*LockStatus)( M24_Object_t *, uint8_t * );
  void                          *pData;
} M24_EEPROM_CommonDrv_t;

/**
 * @brief  M24 driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M24_Object_t * );
  int32_t       (*DeInit)( M24_Object_t * );
  int32_t       (*IsReady)( M24_Object_t *, const uint32_t );
  int32_t       (*ReadRegister)(M24_Object_t *, uint8_t * const, const uint8_t );
  int32_t       (*WriteRegister)(M24_Object_t *, const uint8_t, const uint8_t );  
  int32_t       (*ReadByte)(M24_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*WriteByte)(M24_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M24_Object_t *, uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M24_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t );  
  int32_t       (*ReadData)(M24_Object_t *, uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M24_Object_t *, uint8_t * , const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteID)(M24_Object_t *, uint8_t *, const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*ReadID)(M24_Object_t *, uint8_t * , const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*LockID)(M24_Object_t *); 
  int32_t       (*LockStatus)(M24_Object_t *, uint8_t * );
  void                          *pData;
} M24_Drv_t;


/** @addtogroup M24_Imported_Variables
 * @{
 */

extern M24_Drv_t M24_i2c_Drv;

/* Exported macro ------------------------------------------------------------*/
/* Imported functions ------------------------------------------------------- */
/** @addtogroup M24xx_and_M95xx__Imported_Functions
 * @{
 */
int32_t M24_RegisterBusIO(M24_Object_t *pObj,M24_IO_t *pIO);


/** @addtogroup M24xx_Private_Functions
 * @}
 */
/* Private function prototypes -----------------------------------------------*/
int32_t M24_i2c_Init( M24_Object_t * pObj );
int32_t M24_i2c_DeInit(M24_Object_t * pObj );
int32_t M24_i2c_IsDeviceReady(M24_Object_t * pObj, const uint32_t Trials );
int32_t M24_i2c_ReadByte( M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr);
int32_t M24_i2c_ReadPage(M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize );
int32_t M24_i2c_ReadData( M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t NbByte);
int32_t M24_i2c_WriteByte(M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr);
int32_t M24_i2c_WritePage( M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize,const uint16_t NByte);
int32_t M24_i2c_WriteData(M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize, const uint16_t NbByte );
int32_t M24_i2c_WriteID(M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize, const uint16_t NbByte );
int32_t M24_i2c_ReadID(M24_Object_t * pObj, uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize, const uint16_t NbByte );
int32_t M24_i2c_LockID( M24_Object_t * pObj ); 
int32_t M24_i2c_LockStatus(M24_Object_t * pObj,uint8_t * pData );
 


/* Exported functions ------------------------------------------------------- */

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
#endif /* __M_24xx_H */

