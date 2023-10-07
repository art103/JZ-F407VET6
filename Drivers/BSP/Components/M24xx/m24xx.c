/**
  ******************************************************************************
  * @file    m24xx.c
  * @author  SRA-SAIL, STM32ODE
  * @brief   file provides functionality of driver functions and to manage communication 
             between BSP and M24xx chip
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
#include "m24xx.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */ 


/** @defgroup M24
  * @brief This file provides set of driver functions to manage communication
  *        between BSP and M24xx chip.
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup M24_Private_Defines
  * @{
  */  
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/


/* Global variables ---------------------------------------------------------*/
/** @defgroup M24_Global_Variables
  * @{
  */

M24_Drv_t M24_i2c_Drv =
{
  M24_i2c_Init,
  M24_i2c_DeInit,
  M24_i2c_IsDeviceReady,
  NULL,
  NULL,
  M24_i2c_ReadByte,
  M24_i2c_WriteByte,
  M24_i2c_ReadPage,
  M24_i2c_WritePage,  
  M24_i2c_ReadData,
  M24_i2c_WriteData,
  M24_i2c_WriteID,
  M24_i2c_ReadID,
  M24_i2c_LockID, 
  M24_i2c_LockStatus,
  NULL
};
/**
  * @}
  */ 

/* Public functions ---------------------------------------------------------*/
/** @defgroup M24_Public_Functions
  * @{
  */

/*!
 * @brief IO function pointer structure
 */

/* -------- Static functions prototypes --------------------------------------------- */
/* Exported functions ---------------------------------------------------------*/
int32_t M24_RegisterBusIO(M24_Object_t *pObj, M24_IO_t *pIO)
{
  pObj->IO.Address	   = pIO->Address;
  pObj->IO.Init            = pIO->Init;
  pObj->IO.DeInit          = pIO->DeInit;
  pObj->IO.ReadReg 	   = pIO->ReadReg;
  pObj->IO.WriteReg        = pIO->WriteReg;
  pObj->IO.ReadReg16 	   = pIO->ReadReg16;
  pObj->IO.WriteReg16      = pIO->WriteReg16;
  pObj->IO.Transmit        = pIO->Transmit;
  pObj->IO.IsReady	   = pIO->IsReady;
  pObj->IO.Delay           = pIO->Delay;
  	
  
  if(!pObj->IO.Init)
  {
    return M24_ERROR;
  }
  return M24_OK;
}
/**
  * @brief  Set M24 eeprom Initialization
  * @param  None
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_Init( M24_Object_t *pObj )
{ 
  /* Configure the low level interface */
  if (pObj->IO.Init()<0)
  {
    return M24_ERROR;
  }
  return M24_OK;
}

int32_t M24_i2c_DeInit( M24_Object_t *pObj )
{ 
 if (pObj->IO.DeInit()< 0)
 {
   return M24_ERROR;
 }
  /* Configure the low level interface */
  return M24_OK;
}

/**
  * @brief  Check M24 availability
  * @param  Trials : number of max tentative tried
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_IsDeviceReady(M24_Object_t *pObj, const uint32_t Trials )
{
  /* Test i2c with M24 */
  return pObj->IO.IsReady( pObj->IO.Address, Trials );
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  TargetName : Device Address of selected memory    
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadByte( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr)
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }  
  
  if (pObj->IO.Address == 0xA8)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, 1);
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, 1);
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadPage( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, const uint16_t PageSize )
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, PageSize );
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, PageSize );
}

/**
  * @brief  Read N bytes starting from specified I2C address
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to read
  * @param  DeviceAddr : Device Address of selected memory
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadData(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr, 
                           const uint16_t NbByte )
{  
  /* Before calling this function M24xx must be ready, here is a check to detect an issue */
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
    return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8)
    return pObj->IO.ReadReg(pObj->IO.Address, TarAddr, pData, NbByte );
  else
    return pObj->IO.ReadReg16(pObj->IO.Address, TarAddr, pData, NbByte );
}

/**
  * @brief  Write a single byte to a specified address of I2C memory
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  DeviceAddr : Device Address of selected memory
  * @retval EEPROMEX enum status
*/
int32_t M24_i2c_WriteByte( M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr)
{
  int32_t status;
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
    {
      return M24_TIMEOUT;
    }
  
  if (pObj->IO.Address == 0xA8)
    status = pObj->IO.WriteReg(pObj->IO.Address, TarAddr, pData, 1 );
  else
    status = pObj->IO.WriteReg16(pObj->IO.Address, TarAddr, pData, 1 );
  
  while( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS )  != M24_OK ) {};  
  return status;
}

/**
  * @brief  Write Page
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  DeviceAddr : Device Address of selected memory
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
*/
int32_t M24_i2c_WritePage(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                          const uint16_t PageSize,const uint16_t NByte)
{
  int32_t status;
  if ( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK)
  {
    return M24_TIMEOUT;
  }    
  
  
  if (pObj->IO.Address == 0xA8)
    status = pObj->IO.WriteReg(pObj->IO.Address, TarAddr, pData, PageSize);
  else
    status = pObj->IO.WriteReg16(pObj->IO.Address, TarAddr, pData, PageSize);
  while( pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) 
  {
  };
    return status; 
}

/**
  * @brief  Write N data bytes starting from specified I2C Address
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  DeviceAddr : Device Address of selected memory
  * @param  PageSize : Size of the page of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
  */

int32_t M24_i2c_WriteData(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                          const uint16_t PageSize, const uint16_t Size )
{
  
 uint32_t iNumberOfPage;
  int32_t status = M24_OK;
  uint32_t targetAddress = TarAddr;
  /*to handle dynamically start writing address*/
  if (targetAddress >= PageSize)
    {
     iNumberOfPage =  Size / PageSize;
    if ((targetAddress % PageSize) > 0)
      {
      iNumberOfPage += 1;
      } 
    }
  else  
    {
    iNumberOfPage = ( targetAddress + Size ) / PageSize;
    }
  
  uint32_t iRemainder = ( targetAddress + Size ) % PageSize;
  uint8_t * pageIndex = pData;
    
 if (iRemainder>0)
  {
    iNumberOfPage += 1;
  }
  
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }  
  
  if (targetAddress == 0)       /*If target address from which read/write will be done starts from 0*/  
  {
    for (int index = 0;index < iNumberOfPage;index++)
    {   
       uint32_t iSize = PageSize;
       if (index+1 == iNumberOfPage)     /*For aligning last page of eeprom*/
        iSize = iRemainder;
          
       
       if (pObj->IO.Address == 0xA8)
        status = pObj->IO.WriteReg(pObj->IO.Address, targetAddress,  pageIndex, iSize );
       else
         status = pObj->IO.WriteReg16(pObj->IO.Address, targetAddress,  pageIndex, iSize );
       
        pObj->IO.Delay(6);
        targetAddress += iSize;
        pageIndex += iSize;
        while (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {}; 
        
     }
     return status;    
  }
  else
  {
    for(int index = 0;index < iNumberOfPage;index++)
    {
        uint32_t iSize = PageSize;
       if (index == 0) /*For aligning first page*/
        { 
          if (targetAddress <= PageSize)
            iSize = (PageSize - targetAddress)>0? (PageSize - targetAddress) : PageSize;
          else
            iSize = PageSize - (targetAddress % PageSize); 
        }
          
        if (index+1 == iNumberOfPage) /*For aligning last page of eeprom*/
          iSize = iRemainder;
         
       if (pObj->IO.Address == 0xA8)
    	   status = pObj->IO.WriteReg(pObj->IO.Address, targetAddress,  pageIndex, iSize );
       else
    	   status = pObj->IO.WriteReg16(pObj->IO.Address, targetAddress,  pageIndex, iSize );
       
        pObj->IO.Delay(6);
        targetAddress += iSize;
        pageIndex += iSize;
                   
        while (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {}; 
        
     }
     return status;     
  }  
}

/**
  * @brief  Write Identification Page
  * @param  pData : pointer of the data to write
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  DeviceAddr : Device Address of selected memory
  * @param  NbByte : number of bytes to write
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_WriteID(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                        const uint16_t PageSize, const uint16_t NbByte ){
  int32_t status;
  uint16_t  Devaddr;
  uint16_t mem_addr;
  uint16_t  count;
  uint16_t temp;
  uint16_t  bitcount;
  uint16_t mask = 0 ;
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  uint8_t *pdata_index = (uint8_t *)pData;
  Devaddr = pObj->IO.Address;
  bitcount = BITCOUNT;
  temp = PageSize;
  mem_addr = ( uint16_t )TarAddr;
  Devaddr |= 1 << IDMASK;                  /* Set fifth bit of Device address */
  while ( temp / ( 1 << bitcount ) != 0 ){  /* Generate mask for address*/
   mask |= ( 1 << (bitcount - 1) );
   bitcount++;
  }
  mem_addr &= mask;                        /* Mask address address according to pagesize*/  
  count = PageSize - mem_addr % PageSize;  /* Calculate available space in the ID page */
  if ( NbByte <= count ){  /* Address byte is not aligned with page and no byte must be less than available byte in ID page*/
    if (pObj->IO.Address == 0xA8)
        status = pObj->IO.WriteReg(Devaddr, mem_addr,  pdata_index, NbByte );
    else
        status = pObj->IO.WriteReg16(Devaddr, mem_addr,  pdata_index, NbByte );
  }
  else 
    return M24_ERROR;   /* Return error if above two condtions does'nt met */
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {};
  return status; 
}

/**
  * @brief  Read Identification Page 
  * @param  pData : pointer of the data to store
  * @param  TarAddr : I2C data memory address to write
  * @param  PageSize : Size of the page of selected memory
  * @param  DeviceAddr : Device Address of selected memory
  * @param  NbByte : number of bytes to read
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_ReadID(M24_Object_t *pObj, uint8_t * pData, const uint32_t TarAddr,
                                       const uint16_t PageSize, const uint16_t NbByte )
{
  uint16_t  Devaddr;
  uint16_t mem_addr;
  uint16_t count;
  uint16_t temp;
  uint8_t bitcount;
  uint16_t mask;
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS )!= M24_OK )
    return M24_TIMEOUT;
  mem_addr = ( uint16_t )TarAddr;
  Devaddr = pObj->IO.Address;
  mask = 0;
  bitcount = BITCOUNT;
  temp = PageSize;
  Devaddr |= 1 << IDMASK ;
  while ( temp / ( 1 << bitcount ) != 0 )
  {
   mask |= ( 1 << (bitcount - 1) );
   bitcount++;
  }
  mem_addr &= mask;
  count = PageSize - mem_addr % PageSize;
  if (NbByte <= count)
    if (pObj->IO.Address == 0xA8)
      return pObj->IO.ReadReg( Devaddr, TarAddr, pData, NbByte);
    else
      return pObj->IO.ReadReg16( Devaddr, TarAddr, pData, NbByte);
  else
    if (pObj->IO.Address == 0xA8)
      return pObj->IO.ReadReg(Devaddr, TarAddr,  pData, count);
    else
      return pObj->IO.ReadReg16(Devaddr, TarAddr,  pData, count);
}

/**
  * @brief  Permanentaly lock identification page of memory 
  * @param  DeviceAddr : Device Address of selected memory
  * @retval BSP status
  */
int32_t M24_i2c_LockID( M24_Object_t *pObj )
{  
  int32_t status = M24_OK;
  uint8_t  Devaddr = pObj->IO.Address | 1 << IDMASK;
  uint8_t lid_cmd = LOCKID;
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  
  if (pObj->IO.Address == 0xA8)
    status = pObj->IO.WriteReg(Devaddr, ADDRLID_16, &lid_cmd, 1 );
  else
    status = pObj->IO.WriteReg16(Devaddr, ADDRLID_16, &lid_cmd, 1 );
  
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK ) {};
  return status;
  
}

/**
  * @brief  Reads the lock status
  * @param  pData : pointer of the data to read
  * @param  DeviceAddr : DeviceAddress of the selected memory
  * @retval EEPROMEX enum status
  */
int32_t M24_i2c_LockStatus(M24_Object_t *pObj, uint8_t * pData )
{
  uint8_t dummybyte = 0xAA;
  uint8_t  Devaddr = pObj->IO.Address | 1 << IDMASK;
  if (  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK )
  {
    return M24_TIMEOUT;
  }
  if ( pObj->IO.Transmit(Devaddr, &dummybyte , 1 ) == M24_OK )
  {
    *pData = 0;
  }
  else 
  {
    *pData = 1;  
  } 
  while(  pObj->IO.IsReady( pObj->IO.Address, MIN_TRIALS ) != M24_OK );  
  return M24_OK;
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

/**
 * @}
 */

