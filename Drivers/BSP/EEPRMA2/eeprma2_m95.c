/**
  ******************************************************************************
  * @file    eeprma2_m95.c
  * @author  SRA-SAIL, STM32ODE
  * @brief   This file provides X-NUCLEO-EEPRMA2 Board 
  *          specific functions
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
/* Includes ------------------------------------------------------------------*/
#include "eeprma2_m95.h"

/** @addtogroup BSP
 * @{
 */
static M95_EEPROM_CommonDrv_t *M95Drv[EEPRMA2_M95_INSTANCES_NBR];
void *M95CompObj[EEPRMA2_M95_INSTANCES_NBR];


/** @defgroup M24 EEPROM Private Function Prototypes
 * @{
 */

static int32_t M95M04_0_Probe(void);
static int32_t M95256_0_Probe(void);
static int32_t M95040_0_Probe(void);

/* Global variables ----------------------------------------------------------*/
EEPROMEX_CtrlPin_TypeDef EEPROMEX_CtrlPin[7] = { { M95M04_EEPROM_SPI_CS_PIN, M95M04_EEPROM_SPI_CS_PORT },
                                                    { M95256_EEPROM_SPI_CS_PIN, M95256_EEPROM_SPI_CS_PORT },
                                                    { M95040_EEPROM_SPI_CS_PIN, M95040_EEPROM_SPI_CS_PORT },
                                                    { EEPROMEX_SLAVE_FOUR_PIN, EEPROMEX_SLAVE_FOUR_PIN_PORT },
                                                    { EEPRMA2_M24_WC_PIN, EEPRMA2_M24_WC_PORT},
                                                    { EEPRMA2_M95_HOLD_PIN, EEPRMA2_M95_HOLD_PORT },
                                                    { EEPRMA2_M95_WP_PIN, EEPRMA2_M95_WP_PORT } }; 

uint16_t M95PageSize   = 0;
uint64_t M95MemorySize = 0; /*in bytes*/

/**
 * @brief  Initializes the SPI EEPROMs
 * @param  Instance SPI EEPROMs instance to be used
 * @retval BSP status
 */
int32_t EEPRMA2_M95_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  switch (Instance)
  {
   case EEPRMA2_M95M04_0:
    if (M95M04_0_Probe() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
    break;
   case EEPRMA2_M95256_0:
    if (M95256_0_Probe() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
    break;
   case EEPRMA2_M95040_0:
    if (M95040_0_Probe() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
    break;
   default:
    ret = BSP_ERROR_WRONG_PARAM;
    break;
  }
  
  if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }
  return ret;
}  

/**
 * @brief  Deinitialize environmental sensor
 * @param  Instance environmental sensor instance to be used
 * @retval BSP status
 */
int32_t EEPRMA2_M95_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= EEPRMA2_M95_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (M95Drv[Instance]->DeInit(M95CompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  This function give high on selected control pin
  * @param  pin : selected control pin
  * @retval None
  */
void EEPROMEX_CTRL_HIGH(const uint8_t pin ){
  HAL_GPIO_WritePin(EEPROMEX_CtrlPin[pin].EEPROMEX_PIN_PORT,EEPROMEX_CtrlPin[pin].EEPROMEX_PIN,GPIO_PIN_SET);
}

/**
  * @brief  This function give low on selected control pin
  * @param  pin : selected control pin
  * @retval None
  */
void EEPROMEX_CTRL_LOW(const uint8_t pin)
{  
  HAL_GPIO_WritePin(EEPROMEX_CtrlPin[pin].EEPROMEX_PIN_PORT,EEPROMEX_CtrlPin[pin].EEPROMEX_PIN,GPIO_PIN_RESET);  
}
/**
 * @}
 */

/**
  * @brief  Read resgister from memory
  * @param  pData   : pointer to the register to read
  * @param  Instance : memory name to read
  * @retval BSP status
  */
int32_t EEPRMA2_M95_ReadReg(uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->ReadRegister(M95CompObj[Instance], pData) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
}

/**
  * @brief  Write resgister to memory
  * @param  pData   : pointer to the register to write
  * @param  Instance : memory name to write
  * @retval BSP status
  */
int32_t EEPRMA2_M95_WriteReg(uint32_t Instance, uint8_t pData)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->WriteRegister(M95CompObj[Instance], pData) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  } 
  return ret; 
}

/**
  * @brief  Reads byte from the  memory at specific address
  * @param  pData   : pointer to the data to read
  * @param  TarAddr : memory address to read
  * @param  Instance : memory name to read
  * @retval BSP status
  */
int32_t EEPRMA2_M95_ReadByte(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr)
{
   int32_t ret;
   EEPRMA2_M95MemorySizeLocator(Instance);
   
   if (M95Drv[Instance]->ReadByte(M95CompObj[Instance], pData, TarAddr) != BSP_ERROR_NONE)
   {
     ret = BSP_ERROR_COMPONENT_FAILURE;
   }
   else
   {
     ret = BSP_ERROR_NONE;
   }
   return ret; 
}

/**
  * @brief  Reads complete page from the memory at page start address
  * @param  Instance : memory name to read
  * @param  pData   : pointer to the data to read       
  * @param  TarAddr : starting page address to read
  * @retval BSP status
  */
int32_t EEPRMA2_M95_ReadPage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size)
{
   int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  if ((TarAddr + Size)>M95MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  uint32_t iNumberOfPage = (TarAddr + Size) / M95PageSize;
  uint32_t iRemainder = (TarAddr + Size) % M95PageSize;
  uint32_t PageAddress = TarAddr * M95PageSize;
  uint32_t iPageNumber = TarAddr;
  if (iRemainder!=0)
  {
    iNumberOfPage+=1;  
  }
  
  if (iNumberOfPage<=1)
  {
    if (M95Drv[Instance]->ReadPage(M95CompObj[Instance], pData, PageAddress,M95PageSize) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  else
  {
    for (uint32_t iCounter=0; iCounter<iNumberOfPage; iCounter++)
    {
      uint32_t iPageAddress = iPageNumber * M95PageSize;
      ret = M95Drv[Instance]->ReadPage(M95CompObj[Instance], &pData[0+iCounter*M95PageSize], iPageAddress,M95PageSize);     
      iPageNumber++;
      HAL_Delay(5);  
    }
  }    
  return ret; 
}

/**
  * @brief  Reads data from the memory
  * @param  Instance : memory name to read
  * @param  pData   : pointer to the data to read       
  * @param  TarAddr : starting address to read
  * @param  Size : Size in bytes of the value to be Read
  * @retval BSP status
  */
int32_t EEPRMA2_M95_ReadData(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr,  const uint16_t Size)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  if ((TarAddr + Size)>M95MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  if (M95Drv[Instance]->ReadData( M95CompObj[Instance], pData, TarAddr, Size) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret; 
}

/**
  * @brief  Writes bytes in the memory at specific address
  * @param  Instance : memory name to write
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @retval EEPROMEX enum status
  */
int32_t EEPRMA2_M95_WriteByte(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  if ((TarAddr + 1)>M95MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  if (M95Drv[Instance]->WriteByte(M95CompObj[Instance], pData, TarAddr) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret; 
}

/**
  * @brief  Writes complete page in the memory at starting page address
  * @param  Instance : memory name to write
  * @param  pData : pointer to the data to write
  * @param  TarAddr : SPI data memory starting page address(page no.) to write
  * @param  Size : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M95_WritePage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size)
{
   int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  if ((TarAddr + Size)>M95MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  uint32_t iPageNumber = TarAddr;
  uint32_t iNumberOfPage = (Size) / M95PageSize;
  uint32_t iRemainder = (Size) % M95PageSize;
   
  if (iRemainder!=0)
  {
    iNumberOfPage+=1;  
  }
  
  if (iNumberOfPage<=1)
  {
    /* Absolute address to write, depending on the page number(TarAddr is the pagenumber)  */
    uint32_t iPageAddress = TarAddr * M95PageSize;
    if (M95Drv[Instance]->WritePage(M95CompObj[Instance], pData, iPageAddress,M95PageSize, Size) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  else
  {
    for (uint32_t iCounter=0; iCounter<iNumberOfPage; iCounter++)
    {
      uint32_t iPageAddress = iPageNumber * M95PageSize;
      ret = M95Drv[Instance]->WritePage(M95CompObj[Instance], &pData[0+iCounter*M95PageSize], iPageAddress,M95PageSize, Size);     
      iPageNumber++;
      HAL_Delay(5);  
    }
  }    
  return ret; 
}

/**
  * @brief  Write data to the memory
  * @param  Instance : memory name to write
  * @param  pData   : pointer to the data to be written       
  * @param  TarAddr : starting address to be written
  * @param  Size : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M95_WriteData(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr,  const uint16_t Size)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  if ((TarAddr + Size)>M95MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  if (M95Drv[Instance]->WriteData(M95CompObj[Instance],  pData, TarAddr, M95PageSize, Size) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
}

/**
  * @brief  Writes data in identification page of the memory at specific address
  * @param  Instance the device instance
  * @param  pData : pointer to the data to write
  * @param  TarAddr : SPI  data memory address to write
  * @retval BSP status
  */
int32_t EEPRMA2_M95_WriteID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size )
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->WriteID(M95CompObj[Instance], pData, TarAddr, M95PageSize, Size ) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret; 
  
}
/**
  * @brief  Reads data in identification page of the memory at specific address
  * @param  Instance : memory name to write
  * @param  pData   : pointer to the data to write
  * @param  TarAddr : memory address to write
  * @param  Size    : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M95_ReadID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size )
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->ReadID( M95CompObj[Instance],pData, TarAddr, M95PageSize, Size ) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret;   
}

/**
  * @brief  Lock status of the memory at specific address
  * @param  pData   : pointer to the data to write
  * @param  Instance : memory name to write
  * @retval BSP status
  */
int32_t EEPRMA2_M95_LockStatus( uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->LockStatus(M95CompObj[Instance], pData ) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  return ret;
}

/**
  * @brief  Lock identification page of the memory
  * @param  Instance : memory name to write
  * @retval BSP status
  */
int32_t EEPRMA2_M95_LockID( uint32_t Instance)
{
  int32_t ret;
  EEPRMA2_M95MemorySizeLocator(Instance);
  
  if (M95Drv[Instance]->LockID(M95CompObj[Instance]) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  
  return ret;
}


int32_t EEPRMA2_SPI_ReadReg(uint8_t * pData, uint8_t Devaddr)
{
 EEPROMEX_CTRL_LOW( Devaddr & EEPROMEX_SPI_SLAVESEL );
 if ( EEPRMA2_M95_IOWrite( EEPROMEX_RDSR ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  if ( EEPRMA2_SPI_Recv( ( uint8_t * )pData, 1 ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  EEPROMEX_CTRL_HIGH( Devaddr & EEPROMEX_SPI_SLAVESEL );
  return BSP_ERROR_NONE; 
}

int32_t EEPRMA2_SPI_RecvBuffer(uint8_t * pData, uint32_t TarAddr, 
                               uint32_t DevAddr, const uint16_t Size, const uint8_t Inst )
{
  EEPROMEX_CTRL_LOW( DevAddr & EEPROMEX_SPI_SLAVESEL );
  if (EEPRMA2_M95_IOWrite(Inst) != BSP_ERROR_NONE)
    return BSP_ERROR_COMPONENT_FAILURE;
  if (EEPRMA2_M95_WriteAddr(TarAddr, DevAddr) != BSP_ERROR_NONE ) 
    return BSP_ERROR_COMPONENT_FAILURE;
  if (EEPRMA2_SPI_Recv( pData, Size ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  EEPROMEX_CTRL_HIGH( DevAddr & EEPROMEX_SPI_SLAVESEL );
  return BSP_ERROR_NONE;
}

/**
  * @brief  Write data, at specific address, through spi to the M95xx
  * @param  pData: pointer to the data to write
  * @param  DevAddr : Target device address
  * @param  TarAddr : I2C data memory address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval EEPROMEX enum status
  */
int32_t EEPRMA2_SPI_SendBuffer(uint8_t * pData,uint32_t TarAddr, uint32_t DevAddr, 
                                    const uint16_t Size, const uint8_t Inst)
{
  if ( EEPRMA2_M95_WriteCmd( EEPROMEX_WREN, DevAddr  ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE; 
  EEPROMEX_CTRL_LOW( DevAddr & EEPROMEX_SPI_SLAVESEL );
  if ( EEPRMA2_M95_IOWrite(Inst) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  if ( EEPRMA2_M95_WriteAddr(TarAddr, DevAddr) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  if ( EEPRMA2_SPI_Send( pData, Size )!= BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  EEPROMEX_CTRL_HIGH( DevAddr & EEPROMEX_SPI_SLAVESEL );
  return BSP_ERROR_NONE;
}


int32_t EEPRMA2_SPI_WriteReg(uint8_t pData, uint32_t Devaddr)
{
  EEPROMEX_CTRL_LOW( Devaddr & EEPROMEX_SPI_SLAVESEL ); 
  if ( EEPRMA2_M95_IOWrite( EEPROMEX_WREN ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  if (EEPRMA2_M95_IOWrite( EEPROMEX_WRSR ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  if (EEPRMA2_M95_IOWrite( pData ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  EEPROMEX_CTRL_HIGH( Devaddr & EEPROMEX_SPI_SLAVESEL );
  EEPRMA2_M95_IsDeviceReady( Devaddr);
  return BSP_ERROR_NONE;
}


/**
* @brief  Checks if target device is ready for communication
* @note   This function is used with Memory devices
* @param  DevAddr : Target device address
* @param  Trials : Number of trials
* @retval EEPROMEX enum status
*/
int32_t EEPRMA2_M95_IsDeviceReady(uint8_t Devaddr)
{
  uint8_t status = 0;
  EEPROMEX_CTRL_LOW( Devaddr & EEPROMEX_SPI_SLAVESEL );
  if ( EEPRMA2_M95_IOWrite( EEPROMEX_RDSR ) != BSP_ERROR_NONE )
    return BSP_ERROR_COMPONENT_FAILURE;
  do {
    EEPRMA2_SPI_Recv( &status, 1 );
  } while ( (status & EEPROMEX_SPI_WIPFLAG) == 1 );
  EEPROMEX_CTRL_HIGH( Devaddr & EEPROMEX_SPI_SLAVESEL );
  return BSP_ERROR_NONE;
}
/**
  * @brief  Write one byte to the memory .
  * @param  TxData: Byte to be send 
  * @retval EEPROMEX enum status
  */
int32_t EEPRMA2_M95_IOWrite(uint8_t TxData )
{
  int32_t status = BSP_ERROR_NONE;
  status = EEPRMA2_SPI_Send( &TxData, 1);
  return status;
}

/**
  * @brief  Write commands to target memory.
  * @param  Cmd : Command to be send 
  * @param  DevAddr : Target device address
  * @retval EEPROMEX enum status
  */
int32_t EEPRMA2_M95_WriteCmd( uint8_t Cmd, uint8_t Devaddr)
{
  int32_t status = BSP_ERROR_NONE; 
  EEPROMEX_CTRL_LOW( Devaddr & EEPROMEX_SPI_SLAVESEL );         /* For M95M04  0xCC & 0x03 = 0*/
  status = EEPRMA2_SPI_Send( &Cmd, 1 );
  EEPROMEX_CTRL_HIGH( Devaddr & EEPROMEX_SPI_SLAVESEL );
  return status;
}

/**
  * @brief  Send memory address to target memory 
  * @param  TarAddr : SPI data memory address to read  
  * @param  DevAddr : Target device address
  * @retval EEPROMEX enum status
  */
int32_t EEPRMA2_M95_WriteAddr( const uint32_t TarAddr, const uint32_t DevAddr)
{
  uint8_t AddrSize; 
  static uint8_t Addr[3]; 
  uint8_t count, temp;
  uint32_t addrpacket = TarAddr; 
  AddrSize = ( DevAddr & EEPROMEX_SPI_ADDRSEL ) >> EEPROMEX_SPI_ADDRSHIFT;
  for (count = AddrSize; count > 0; count--){
    temp = (uint8_t)( addrpacket & 0xFF );
    Addr[count - 1] = temp;
    addrpacket = addrpacket >> 8;
  }
  return  EEPRMA2_SPI_Send( Addr, AddrSize );
}

/**
 * @brief  Register Bus IOs for instance M95M04 if component ID is OK
 * @retval BSP status
 */
static int32_t M95M04_0_Probe(void)
{
  M95_IO_t io_ctxm04;
  int32_t ret = BSP_ERROR_NONE;
 static M95_Object_t M95M04_obj_0;
  io_ctxm04.Address        = M95M04_SPI_ADDR;
  io_ctxm04.Init           = EEPRMA2_SPI_Init;
  io_ctxm04.DeInit         = EEPRMA2_SPI_DeInit;
  io_ctxm04.Read           = EEPRMA2_SPI_ReadReg;
  io_ctxm04.Write          = EEPRMA2_SPI_WriteReg;
  io_ctxm04.WriteBuffer    = EEPRMA2_SPI_SendBuffer;
  io_ctxm04.ReadBuffer     = EEPRMA2_SPI_RecvBuffer;
  io_ctxm04.IsReady        = EEPRMA2_M95_IsDeviceReady;
  io_ctxm04.Delay          = EEPRMA2_M95_Delay;

  if (M95_RegisterBusIO(&M95M04_obj_0,&io_ctxm04) != M95_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  
  M95CompObj[EEPRMA2_M95M04_0] = &M95M04_obj_0;
  M95Drv[EEPRMA2_M95M04_0] = (M95_EEPROM_CommonDrv_t *)(void *)&M95_spi_Drv;
  
  if (M95Drv[EEPRMA2_M95M04_0]->Init(M95CompObj[EEPRMA2_M95M04_0]) != M95_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  else
    {
      ret = BSP_ERROR_NONE;
    }  
  return ret;
}

/**
 * @brief  Register Bus IOs for instance M95256 if component ID is OK
 * @retval BSP status
 */
static int32_t M95256_0_Probe(void)
{
  M95_IO_t io_ctx256;
  int32_t ret = BSP_ERROR_NONE;
 static M95_Object_t M95256_obj_0;
  io_ctx256.Address        = M95256_SPI_ADDR;
  io_ctx256.Init           = EEPRMA2_SPI_Init;
  io_ctx256.DeInit         = EEPRMA2_SPI_DeInit;
  io_ctx256.Read           = EEPRMA2_SPI_ReadReg;
  io_ctx256.Write          = EEPRMA2_SPI_WriteReg;
  io_ctx256.WriteBuffer    = EEPRMA2_SPI_SendBuffer;
  io_ctx256.ReadBuffer     = EEPRMA2_SPI_RecvBuffer;
  io_ctx256.IsReady        = EEPRMA2_M95_IsDeviceReady;
  io_ctx256.Delay          = EEPRMA2_M95_Delay;
  
  if (M95_RegisterBusIO(&M95256_obj_0, &io_ctx256) != M95_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  
  M95CompObj[EEPRMA2_M95256_0] = &M95256_obj_0;
  
  M95Drv[EEPRMA2_M95256_0] = (M95_EEPROM_CommonDrv_t *)(void *)&M95_spi_Drv;

  if (M95Drv[EEPRMA2_M95256_0]->Init(M95CompObj[EEPRMA2_M95256_0]) != M95_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  else
    {
      ret = BSP_ERROR_NONE;
    }  
  return ret;
}

/**
 * @brief  Register Bus IOs for instance M95040 if component ID is OK
 * @retval BSP status
 */
static int32_t M95040_0_Probe(void)
{
  M95_IO_t io_ctx040;
  int32_t ret = BSP_ERROR_NONE;
  static M95_Object_t M95040_obj_0;
  
  io_ctx040.Address        = M95040_SPI_ADDR;
  io_ctx040.Init           = EEPRMA2_SPI_Init;
  io_ctx040.DeInit         = EEPRMA2_SPI_DeInit;
  io_ctx040.Read           = EEPRMA2_SPI_ReadReg;
  io_ctx040.Write          = EEPRMA2_SPI_WriteReg;
  io_ctx040.WriteBuffer    = EEPRMA2_SPI_SendBuffer;
  io_ctx040.ReadBuffer     = EEPRMA2_SPI_RecvBuffer;
  io_ctx040.IsReady        = EEPRMA2_M95_IsDeviceReady;
  io_ctx040.Delay          = EEPRMA2_M95_Delay;

  if (M95_RegisterBusIO(&M95040_obj_0, &io_ctx040) != M95_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }

  M95CompObj[EEPRMA2_M95040_0] = &M95040_obj_0;
  
  M95Drv[EEPRMA2_M95040_0] = (M95_EEPROM_CommonDrv_t *)(void *)&M95_spi_Drv;

 if (M95Drv[EEPRMA2_M95040_0]->Init(M95CompObj[EEPRMA2_M95040_0]) != M95_OK) 
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  else
    {
      ret = BSP_ERROR_NONE;
    }  
  return ret;
}

/**
  * @brief  M95 GPIO Pin Init 
  *         This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration   
  *           - NVIC configuration for DMA interrupt request enable
  * @param  None
  * @retval None
  */
void EEPRMA2_GPIO_Init( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  EEPRMA2_M95_HOLD_RCC(); 
  M95M04_EEPROM_SPI_CS_RCC();
  M95256_EEPROM_SPI_CS_RCC();
  M95040_EEPROM_SPI_CS_RCC();
  EEPROMEX_SPI_SLAVE_FOUR_GPIO_CLK_ENABLE();
  
  
    /* EEPROMEX Write protect pin configuration */
  GPIO_InitStruct.Pin       = EEPRMA2_M95_WP_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( EEPRMA2_M95_WP_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( EEPRMA2_M95_WP_PORT,EEPRMA2_M95_WP_PIN,GPIO_PIN_SET );

      /* EEPROMEX Write protect pin configuration */
  GPIO_InitStruct.Pin       = EEPRMA2_M24_WC_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( EEPRMA2_M24_WC_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( EEPRMA2_M24_WC_PORT,EEPRMA2_M24_WC_PIN,GPIO_PIN_SET );
  
    /* EEPROMEX HOLD pin configuration */
  GPIO_InitStruct.Pin       = EEPRMA2_M95_HOLD_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( EEPRMA2_M95_HOLD_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( EEPRMA2_M95_HOLD_PORT,EEPRMA2_M95_HOLD_PIN,GPIO_PIN_SET );
    /* EEPROMEX Slave select pin configuration */
  /* Slave one : M95M04 for EEPRMA2 or M95M01 for EEPRMA1*/
  GPIO_InitStruct.Pin       = M95M04_EEPROM_SPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( M95M04_EEPROM_SPI_CS_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( M95M04_EEPROM_SPI_CS_PORT,M95M04_EEPROM_SPI_CS_PIN,GPIO_PIN_SET );
  /*Slave two : M95256*/
  GPIO_InitStruct.Pin       = M95256_EEPROM_SPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( M95256_EEPROM_SPI_CS_PORT, &GPIO_InitStruct );  
  HAL_GPIO_WritePin( M95256_EEPROM_SPI_CS_PORT,M95256_EEPROM_SPI_CS_PIN,GPIO_PIN_SET );
  /*Slave three : M95040*/
  GPIO_InitStruct.Pin       = M95040_EEPROM_SPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( M95040_EEPROM_SPI_CS_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( M95040_EEPROM_SPI_CS_PORT,M95040_EEPROM_SPI_CS_PIN,GPIO_PIN_SET );
  /*Slave four : M95XX*/
  GPIO_InitStruct.Pin       = EEPROMEX_SLAVE_FOUR_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( EEPROMEX_SLAVE_FOUR_PIN_PORT, &GPIO_InitStruct );
  HAL_GPIO_WritePin( EEPROMEX_SLAVE_FOUR_PIN_PORT,EEPROMEX_SLAVE_FOUR_PIN,GPIO_PIN_SET );

}

/**
  * @brief  set the atributes of selected memory ( memory size in bytes )
  * @param  Instance : memory name to write

  */
void EEPRMA2_M95MemorySizeLocator(uint32_t Instance)
{
  switch (Instance)
  {    
    case EEPRMA2_M95M04_0:{
     M95PageSize   = 512;        /* Slave Select Pin = ( 1100 1100 & 0000 0011 ) = 0*/
     M95MemorySize = 524288;               /*131072 FOR 1Mb */
    break;
  } 
    case EEPRMA2_M95256_0:{
     M95PageSize   = 64;         /* Slave Select Pin = ( 1100 1001 & 0000 0011 ) = 1 */
     M95MemorySize = 32768;
    break;
  }
    case EEPRMA2_M95040_0:{
     M95PageSize   = 16;        /* Slave Select Pin = ( 1100 1010 & 0000 0011 ) = 2*/
     M95MemorySize = 512;   
    break;
  }
    case EEPRMA2_M95xx :{
#ifdef M95XX  
    switch(M95XX){
    case 2048:{
      M95PageSize   = 256;      /* Slave Select Pin = ( 1100 1111 & 0000 0011 ) = 3*/
      M95MemorySize = 262144;
      break;
    }
    case 1024:{
      M95PageSize   = 256;      /* Slave Select Pin = ( 1100 1111 & 0000 0011 ) = 3 */
      M95MemorySize = 131072;
      break;
    }
    case 512 :{
      M95PageSize   = 128;      /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3 */
      M95MemorySize = 65536;
      break;
    }
    case 256 :{
      M95PageSize   = 64;       /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3  */
      M95MemorySize = 32768;
      break;
    }
    case 128 :{
      M95PageSize   = 64;      /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3  */
      M95MemorySize = 16384;
      break;
    }
    case 64  :{
      M95PageSize   = 32;      /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3 */
      M95MemorySize = 8192;
      break;
    }
    case 32  :{
      M95PageSize   = 32;      /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3 */
      M95MemorySize = 4096;
      break;
    }
    case 16  :{
      M95PageSize   = 32;      /* Slave Select Pin = ( 1100 1011 & 0000 0011 ) = 3  */
      M95MemorySize = 2048;
      break;
    }
    case 8   :{
      M95PageSize   = 32;
      M95MemorySize = 1024;
    }
    case 4   :{
      M95PageSize   = 16;      /* Slave Select Pin = ( 1100 0111 & 0000 0011 ) = 3  */
      M95MemorySize = 512;
      break;
    }
    case 2   :{
      M95PageSize   = 16;      /* Slave Select Pin = ( 1100 0111 & 0000 0011 ) = 3 */ 
      M95MemorySize = 256;
      break;
    }
    default  :{
      M95PageSize   = 16;      /* Slave Select Pin = ( 1100 0111 & 0000 0011 ) = 3 */
      M95MemorySize = 128;
      break;
    }
    }  
#endif
  }
 }
}


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

