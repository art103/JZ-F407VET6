/**
  ******************************************************************************
  * @file    m95xx.h
  * @author  SRA-SAIL, STM32ODE
  * @brief   file provides functionality of driver functions and to manage communication 
             between BSP and M95xx chip
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
#ifndef __M_95XX_H
#define __M_95XX_H

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
/** @addtogroup M95
  * @{
  */

/* Exported types ------------------------------------------------------------*/



/* Exported constants --------------------------------------------------------*/
/** @defgroup M95_Exported_Constants
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
#define LOCKDATA_SPI_M04        0x01
    
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

#define EEPROMEX_SPI_SLAVESEL             0x03U
#define EEPROMEX_SPI_ADDRSEL              0x0CU
#define EEPROMEX_SPI_ADDRSHIFT            0x02U
#define EEPROMEX_SPI_WIPFLAG              0x01U
#define EEPROMEX_SPI_WELFLAG              0x02U
/** @defgroup M95XX error codes
  * @}
  */
  
#define M95_OK              0
#define M95_ERROR   	   -1
#define M95_BUSY    	   -2
#define M95_TIMEOUT 	   -3
#define M95_ADDR_OVERFLOW  -4

/* External variables --------------------------------------------------------*/

/*!
 * @brief Initializes the SPI  object 
 * @param  none
 * @retval Error code: 0 ok, negative KO
 */
typedef int32_t     (*M95_Init_Func)(void);   
typedef int32_t     (*M95_DeInit_Func)(void); 
typedef int32_t     (*M95_Write_Func)(uint8_t, uint32_t);
typedef int32_t     (*M95_Transmit_Func)(uint8_t *, uint32_t, uint32_t , 
                               uint16_t , uint8_t);
typedef int32_t     (*M95_Receive_Func)( uint8_t *, uint32_t, uint32_t , 
                               uint16_t , uint8_t);
typedef int32_t     (*M95_Read_Func) (uint8_t* , uint8_t);
typedef int32_t     (*M95_IsReady_Func) (uint8_t);
typedef void        (*M95_Delay)(uint32_t );   

typedef struct
{
  M95_Init_Func            Init;
  M95_DeInit_Func          DeInit;
  M95_Read_Func		   Read;
  M95_Write_Func           Write;
  M95_Transmit_Func        WriteBuffer;
  M95_Receive_Func         ReadBuffer;
  M95_IsReady_Func         IsReady;
  M95_Delay                Delay;
  uint8_t                  Address;
} M95_IO_t;

typedef struct
{
  M95_IO_t        IO;
} M95_Object_t;
/**
 * @brief  EEPROMEX COMMON driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M95_Object_t *);
  int32_t       (*DeInit)( M95_Object_t *);
  int32_t       (*IsReady)(M95_Object_t *);
  int32_t       (*ReadRegister)( M95_Object_t *,uint8_t * );
  int32_t       (*WriteRegister)(M95_Object_t *, uint8_t );  
  int32_t       (*ReadByte)( M95_Object_t *, uint8_t *, const uint32_t);
  int32_t       (*WriteByte)( M95_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M95_Object_t *,uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M95_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t ); 
  int32_t       (*ReadData)( M95_Object_t *, uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M95_Object_t *,uint8_t *, const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteID)( M95_Object_t *,uint8_t *, const uint32_t ,const uint16_t , const uint16_t);
  int32_t       (*ReadID)( M95_Object_t *,uint8_t * , const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*LockID)(M95_Object_t *); 
  int32_t       (*LockStatus)( M95_Object_t *, uint8_t * );
  void                          *pData;
} M95_EEPROM_CommonDrv_t;


/**
 * @brief  M95 driver structure definition
 */
typedef struct
{
  int32_t       (*Init)( M95_Object_t * );
  int32_t       (*DeInit)( M95_Object_t * );
  int32_t       (*IsReady)(M95_Object_t *);
  int32_t       (*ReadRegister)( M95_Object_t *,uint8_t *);
  int32_t       (*WriteRegister)( M95_Object_t *,uint8_t );  
  int32_t       (*ReadByte)(M95_Object_t *, uint8_t *, const uint32_t);
  int32_t       (*WriteByte)(M95_Object_t *, uint8_t * , const uint32_t);
  int32_t       (*ReadPage)(M95_Object_t *,uint8_t * , const uint32_t , const uint16_t );
  int32_t       (*WritePage)(M95_Object_t *, uint8_t * , const uint32_t , const uint16_t ,const uint16_t );  
  int32_t       (*ReadData)( M95_Object_t *,uint8_t * , const uint32_t, const uint16_t);
  int32_t       (*WriteData)(M95_Object_t *,uint8_t * , const uint32_t, const uint16_t, const uint16_t );
  int32_t       (*WriteID)( M95_Object_t *,uint8_t *, const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*ReadID)( M95_Object_t *,uint8_t * , const uint32_t ,const uint16_t , const uint16_t );
  int32_t       (*LockID)(M95_Object_t *); 
  int32_t       (*LockStatus)( M95_Object_t *,uint8_t * );
  void                          *pData;
} M95_Drv_t;



/** @addtogroup M95_Imported_Variables
 * @{
 */
/* EEPROMEX driver structure */
extern M95_Drv_t M95_spi_Drv;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Imported functions ------------------------------------------------------- */

int32_t M95_IO_ReadReg(uint8_t * const pData);

/** @addtogroup M95xx_Private_Functions
 * @{
 */

int32_t M95_RegisterBusIO(M95_Object_t *pObj, M95_IO_t *pIO);
int32_t M95_spi_Init( M95_Object_t *pObj );
int32_t M95_spi_DeInit( M95_Object_t *pObj );
int32_t M95_spi_IsDeviceReady(M95_Object_t *pObj);
int32_t M95_spi_WriteReg(M95_Object_t *pObj,uint8_t pData);
int32_t M95_spi_ReadReg( M95_Object_t *pObj,uint8_t * pData);
int32_t M95_spi_ReadByte(M95_Object_t *pObj,  uint8_t * pData, const uint32_t TarAddr );
int32_t M95_spi_ReadData(M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr, const uint16_t NbByte );
int32_t M95_spi_ReadPage( M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize); 
int32_t M95_spi_WriteByte(M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr);
int32_t M95_spi_WritePage(M95_Object_t *pObj, uint8_t *  pData, const uint32_t TarAddr,const uint16_t PageSize,const uint16_t NByte);
int32_t M95_spi_WriteData(M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize, const uint16_t NByte);
int32_t M95_spi_WriteID( M95_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize,const uint16_t NbByte );
int32_t M95_spi_ReadID( M95_Object_t *pObj,uint8_t * pData, const uint32_t TarAddr,const uint16_t PageSize, const uint16_t NbByte );
int32_t M95_spi_LockID(M95_Object_t *pObj);
int32_t M95_spi_LockStatus( M95_Object_t *pObj,uint8_t * pData);



/**
 * @}
 */

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
#endif /* __M95_XX_H */

