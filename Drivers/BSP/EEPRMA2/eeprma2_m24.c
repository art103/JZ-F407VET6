/**
 ******************************************************************************
  * @file    eeprma2_m24.c
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
#include "eeprma2_m24.h"

/** @addtogroup BSP
 * @{
 */
static M24_EEPROM_CommonDrv_t *M24Drv[EEPRMA2_M24_INSTANCES_NBR];
void *M24CompObj[EEPRMA2_M24_INSTANCES_NBR];
/** @defgroup M24 EEPROM Private Function Prototypes
 * @{
 */

static int32_t M24C02_0_Probe(void);
static int32_t M24256_0_Probe(void);
static int32_t M24M01_0_Probe(void);

/* Global variables ----------------------------------------------------------*/

uint16_t M24PageSize   = 0;
uint64_t M24MemorySize = 0; /*in bytes*/

/**
 * @}
 */

/** @defgroup EEPRMA2_M24_Exported_Functions 
 * @{
 */

/**
 * @brief  Initializes the I2C EEPROM
 * @param  Instance I2C EEPROM instance to be used
 * @retval BSP status
 */
int32_t EEPRMA2_M24_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  switch (Instance)
  {
   case EEPRMA2_M24C02_0:
    if (M24C02_0_Probe() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
    break;
   case EEPRMA2_M24256_0:
    if (M24256_0_Probe() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
    break;
   case EEPRMA2_M24M01_0:
    if (M24M01_0_Probe() != BSP_ERROR_NONE)
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
int32_t EEPRMA2_M24_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= EEPRMA2_M24_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (M24Drv[Instance]->DeInit(M24CompObj[Instance]) != BSP_ERROR_NONE)
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
  * @brief  Checks if the memory is available
  * @param  Trials : Number of trials
  * @retval BSP status
  */
int32_t EEPRMA2_M24_IsDeviceReady( uint32_t Instance, const uint32_t Trials )
{
  int32_t ret;
  
  if (M24Drv[Instance]->IsReady(M24CompObj[Instance], Trials) != BSP_ERROR_NONE)
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
  * @param  pData   : pointer to the data to read
  * @param  TarAddr : memory address to read
  * @param  Instance the device instance
  * @retval BSP status
  */
int32_t EEPRMA2_M24_ReadByte(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr)
{
  int32_t ret;
  
  if (M24Drv[Instance]->ReadByte(M24CompObj[Instance], pData, TarAddr)!= BSP_ERROR_NONE)
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
  * @brief  Reads data in the memory at specific address
  * @param  pData   : pointer to the data to write
  * @param  TarAddr : memory address to write
  * @param  Instance the device instance
  * @param  Size    : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M24_ReadData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret;
  
  if (M24Drv[Instance]->ReadData(M24CompObj[Instance], pData, TarAddr, Size) != BSP_ERROR_NONE)
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
  * @param  pData   : pointer to the data to read       
  * @param  TarAddr : starting page address to read
  * @param  Instance the device instance
  * @retval BSP status
 */
int32_t EEPRMA2_M24_ReadPage(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret = BSP_ERROR_NO_INIT;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if ((TarAddr + Size)> M24MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
   uint32_t iNumberOfPage = (TarAddr + Size) / M24PageSize;
   uint32_t iRemainder = (TarAddr + Size) % M24PageSize;
   
   uint32_t PageAddress = TarAddr * M24PageSize;        
   uint32_t iPageNumber = TarAddr;
   if (iRemainder!=0)
   { 
     iNumberOfPage+=1;   
   }
   
   if (iNumberOfPage<=1)
   {
     if (M24Drv[Instance]->ReadPage(M24CompObj[Instance], pData, PageAddress,M24PageSize) != BSP_ERROR_NONE)
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
       uint32_t iPageAddress = iPageNumber * M24PageSize;
       ret = M24Drv[Instance]->ReadPage(M24CompObj[Instance], &pData[0+iCounter*M24PageSize], iPageAddress,M24PageSize);     
       iPageNumber++;
       HAL_Delay(5);  
     }
   }    
  
  return ret; 
}


/**
  * @brief  Write Byte in the memory at specific address
  * @param  Instance the device instance
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @retval BSP status
  */
int32_t EEPRMA2_M24_WriteByte (uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr)
{
  int32_t ret;
  
  if (M24Drv[Instance]->WriteByte(M24CompObj[Instance], pData, TarAddr) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    
    while ( EEPRMA2_M24_IsDeviceReady(Instance, MIN_TRIALS) != BSP_ERROR_NONE ) {};
    ret = BSP_ERROR_NONE;
  }
  
  return ret; 
  
}


/**
  * @brief  Write Data in the memory at specific address
  * @param  Instance the device instance
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M24_WriteData(uint32_t Instance, uint8_t * const pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if ((TarAddr + Size)> M24MemorySize)
      return BSP_ERROR_WRONG_PARAM;
  
  
  if (M24Drv[Instance]->WriteData(M24CompObj[Instance], pData, TarAddr, M24PageSize, Size) != BSP_ERROR_NONE)
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
  * @brief  Write Data in the memory at specific address
  * @param  Instance the device instance
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  Size : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M24_WritePage(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size)
{
  int32_t ret = BSP_ERROR_NO_INIT;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if ((TarAddr + Size)> M24MemorySize)
    return BSP_ERROR_WRONG_PARAM;
  
  /*Target Address set in pagewrite function is basically the page number */ 
  uint32_t iPageNumber = TarAddr;  
  uint32_t iNumberOfPage = (Size) / M24PageSize;
  uint32_t iRemainder = (Size) % M24PageSize;
  /* PageWrite function should be begin at starting address of the page */
  if (iRemainder!=0)  
  {
    iNumberOfPage +=1;
  }
  
  if (iNumberOfPage<=1)
  {
    /* Absolute address to write, depending on the page number(TarAddr is the pagenumber)  */
    uint32_t iPageAddress = TarAddr * M24PageSize;
    if (M24Drv[Instance]->WritePage(M24CompObj[Instance], pData, iPageAddress,M24PageSize, Size) != BSP_ERROR_NONE)
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
      uint32_t iPageAddress = iPageNumber * M24PageSize;
      ret = M24Drv[Instance]->WritePage(M24CompObj[Instance], &pData[0+iCounter*M24PageSize], iPageAddress,M24PageSize, Size);     
      iPageNumber++;
      HAL_Delay(5);  
    }
  }   
  
  return ret; 
  
}

/**
  * @brief  Writes data in identification page of the memory at specific address
  * @param  Instance the device instance
  * @param  pData : pointer to the data to write
  * @param  TarAddr : I2C data memory address to write
  * @retval BSP status
  */
int32_t EEPRMA2_M24_WriteID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size )
{
  int32_t ret;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if (M24Drv[Instance]->WriteID(M24CompObj[Instance],  pData, TarAddr, M24PageSize, Size ) != BSP_ERROR_NONE)
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
  * @param  Instance the device instance
  * @param  pData   : pointer to the data to write
  * @param  TarAddr : memory address to write
  * @param  Size    : Size in bytes of the value to be written
  * @retval BSP status
  */
int32_t EEPRMA2_M24_ReadID(uint32_t Instance, uint8_t * pData, const uint32_t TarAddr, const uint16_t Size )
{
  int32_t ret;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if (M24Drv[Instance]->ReadID( M24CompObj[Instance], pData, TarAddr, M24PageSize, Size ) != BSP_ERROR_NONE)
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
int32_t EEPRMA2_M24_LockStatus( uint32_t Instance, uint8_t * pData)
{
  int32_t ret;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if (M24Drv[Instance]->LockStatus(M24CompObj[Instance],  pData ) != BSP_ERROR_NONE)
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
int32_t EEPRMA2_M24_LockID( uint32_t Instance)
{
  int32_t ret;
  EEPRMA2_M24MemorySizeLocator(Instance);
  
  if (M24Drv[Instance]->LockID(M24CompObj[Instance])!= BSP_ERROR_NONE)
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
 * @brief  Register Bus IOs for instance M24C02 if component ID is OK
 * @retval BSP status
 */
static int32_t M24C02_0_Probe(void)
{
  M24_IO_t io_ctxc02;
  int32_t ret = BSP_ERROR_NONE;
  static M24_Object_t M24C02_obj_0;
  
  io_ctxc02.Address     = M24C02_I2C_ADDR;
  io_ctxc02.Init        = EEPRMA2_I2C_Init;
  io_ctxc02.DeInit      = EEPRMA2_I2C_DeInit;
  io_ctxc02.ReadReg     = EEPRMA2_I2C_ReadReg;
  io_ctxc02.WriteReg    = EEPRMA2_I2C_WriteReg;
  io_ctxc02.ReadReg16   = EEPRMA2_I2C_ReadReg16;
  io_ctxc02.WriteReg16  = EEPRMA2_I2C_WriteReg16;
  io_ctxc02.Transmit    = EEPRMA2_I2C_Send;
  io_ctxc02.IsReady     = EEPRMA2_I2C_IsReady;	
  io_ctxc02.Delay       = EEPRMA2_M24_Delay;
  if (M24_RegisterBusIO(&M24C02_obj_0, &io_ctxc02) != M24_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }

  M24CompObj[EEPRMA2_M24C02_0] = &M24C02_obj_0;
  M24Drv[EEPRMA2_M24C02_0] = (M24_EEPROM_CommonDrv_t *)(void *)&M24_i2c_Drv;

  if (M24Drv[EEPRMA2_M24C02_0]->Init( M24CompObj[EEPRMA2_M24C02_0]) != M24_OK)
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
 * @brief  Register Bus IOs for instance M24256  if component ID is OK
 * @retval BSP status
 */
static int32_t M24256_0_Probe(void)
{
  M24_IO_t            io_ctx256;
  int32_t                 ret = BSP_ERROR_NONE;
  static M24_Object_t M24256_obj_0;
  
  io_ctx256.Address     = M24256_I2C_ADDR;
  io_ctx256.Init        = EEPRMA2_I2C_Init;
  io_ctx256.DeInit      = EEPRMA2_I2C_DeInit;
  io_ctx256.ReadReg     = EEPRMA2_I2C_ReadReg;
  io_ctx256.WriteReg    = EEPRMA2_I2C_WriteReg;
  io_ctx256.ReadReg16   = EEPRMA2_I2C_ReadReg16;
  io_ctx256.WriteReg16  = EEPRMA2_I2C_WriteReg16;
  io_ctx256.Transmit    = EEPRMA2_I2C_Send;
  io_ctx256.IsReady     = EEPRMA2_I2C_IsReady;	
  io_ctx256.Delay       = EEPRMA2_M24_Delay;
  if (M24_RegisterBusIO(&M24256_obj_0, &io_ctx256) != M24_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  
  M24CompObj[EEPRMA2_M24256_0] = &M24256_obj_0;
  M24Drv[EEPRMA2_M24256_0] = (M24_EEPROM_CommonDrv_t *)(void *)&M24_i2c_Drv;

  if (M24Drv[EEPRMA2_M24256_0]->Init(M24CompObj[EEPRMA2_M24256_0]) != M24_OK)
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
 * @brief  Register Bus IOs for instance M24256  if component ID is OK
 * @retval BSP status
 */
static int32_t M24M01_0_Probe(void)
{
  M24_IO_t            io_ctxm01;
  int32_t                 ret = BSP_ERROR_NONE;
  static M24_Object_t M24M01_obj_0;
  
  io_ctxm01.Address     = M24M01_I2C_ADDR;
  io_ctxm01.Init        = EEPRMA2_I2C_Init;
  io_ctxm01.DeInit      = EEPRMA2_I2C_DeInit;
  io_ctxm01.ReadReg     = EEPRMA2_I2C_ReadReg;
  io_ctxm01.WriteReg    = EEPRMA2_I2C_WriteReg;
  io_ctxm01.ReadReg16   = EEPRMA2_I2C_ReadReg16;
  io_ctxm01.WriteReg16  = EEPRMA2_I2C_WriteReg16;
  io_ctxm01.Transmit    = EEPRMA2_I2C_Send;
  io_ctxm01.IsReady     = EEPRMA2_I2C_IsReady;
  io_ctxm01.Delay       = EEPRMA2_M24_Delay;
  if (M24_RegisterBusIO(&M24M01_obj_0, &io_ctxm01) != M24_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  
  M24CompObj[EEPRMA2_M24M01_0] = &M24M01_obj_0;
  M24Drv[EEPRMA2_M24M01_0] = (M24_EEPROM_CommonDrv_t *)(void *)&M24_i2c_Drv;

  if (M24Drv[EEPRMA2_M24M01_0]->Init(M24CompObj[EEPRMA2_M24M01_0]) != M24_OK)
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
  * @brief  set the atributes of selected memory ( memory size in bytes )
  * @param  Instance : memory name to write

  */
void EEPRMA2_M24MemorySizeLocator(uint32_t Instance)
{
  
  switch (Instance)
  {    
    case EEPRMA2_M24M01_0:{ 
      M24PageSize   = 256;
      M24MemorySize = 131072;
      break;
    }
    case EEPRMA2_M24256_0:{
      M24PageSize   = 64;
      M24MemorySize = 32768;
      break;
    }
    case EEPRMA2_M24C02_0:{
      M24PageSize   = 16;
      M24MemorySize = 256;
      break;
    }
    case EEPRMA2_M24xx :{ 

#ifdef M24XX
      switch(M24XX){
      case 2048:{
        M24PageSize   = 256;
        M24MemorySize = 262144;
        break;
      }
      case 1024:{
        M24PageSize   = 256;
        M24MemorySize = 131072;
        break;
      }
      case 512 :{
        M24PageSize   = 128;
        M24MemorySize = 65536;
        break;
      }
      case 256 :{
        M24PageSize   = 64;
        M24MemorySize = 32768;
        break;
      }
      case 128 :{
        M24PageSize   = 64;
        M24MemorySize = 16384;
        break;
      }
      case 64  :{
        M24PageSize   = 32;
        M24MemorySize = 8192;
        break;
      }
      case 32  :{
        M24PageSize   = 32;
        M24MemorySize = 4096;
        break;
      }
      case 16 :{
        M24PageSize   = 16;
        M24MemorySize = 2048 ;
      }
      case 8   :{
        M24PageSize   = 16;
        M24MemorySize = 1024;
        break;
      }
      case 4   :{
        M24PageSize   = 16;
        M24MemorySize = 512;
        break;
      }
      default  :{
        M24PageSize   = 16;
        M24MemorySize = 0;
        break;      
      }
     
      }
#endif     
      break;
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

