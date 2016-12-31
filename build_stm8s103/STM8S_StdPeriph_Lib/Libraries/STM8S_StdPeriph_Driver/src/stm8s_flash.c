/**
  ******************************************************************************
  * @file    stm8s_flash.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the FLASH peripheral.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_flash.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/**
@code
 This driver provides functions to configure and program the Flash memory of all
 STM8S devices.

 It includes as well functions that can be either executed from RAM or not, and
 other functions that must be executed from RAM otherwise useless.

 The table below lists the functions that can be executed from RAM.

 +--------------------------------------------------------------------------------|
 |   Functions prototypes      |    RAM execution            |     Comments       |
 ---------------------------------------------------------------------------------|
 |                             | Mandatory in case of block  | Can be executed    |
 | FLASH_WaitForLastOperation  | Operation:                  | from Flash in case |
 |                             | - Block programming         | of byte and word   |
 |                             | - Block erase               | Operations         |
 |--------------------------------------------------------------------------------|
 | FLASH_ProgramBlock          |       Exclusively           | useless from Flash |
 |--------------------------------------------------------------------------------|
 | FLASH_EraseBlock            |       Exclusively           | useless from Flash |
 |--------------------------------------------------------------------------------|

 To be able to execute functions from RAM several steps have to be followed.
 These steps may differ from one toolchain to another.
 A detailed description is available below within this driver.
 You can also refer to the FLASH examples provided within the
 STM8S_StdPeriph_Lib package.

@endcode
*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_CLEAR_BYTE    ((uint8_t)0x00)
#define FLASH_SET_BYTE      ((uint8_t)0xFF)
#define OPERATION_TIMEOUT   ((uint16_t)0xFFFF)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/

/** @addtogroup FLASH_Public_functions
  * @{
  */

/**
  * @brief  Unlocks the program or data EEPROM memory
  * @param  FLASH_MemType : Memory type to unlock
  *         This parameter can be a value of @ref FLASH_MemType_TypeDef
  * @retval None
  */
void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType)
{
  /* Check parameter */
  assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
  
  /* Unlock program memory */
  if(FLASH_MemType == FLASH_MEMTYPE_PROG)
  {
    FLASH->PUKR = FLASH_RASS_KEY1;
    FLASH->PUKR = FLASH_RASS_KEY2;
  }
  /* Unlock data memory */
  else
  {
    FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
    FLASH->DUKR = FLASH_RASS_KEY1;
  }
}

/**
  * @brief  Locks the program or data EEPROM memory
  * @param  FLASH_MemType : Memory type
  *         This parameter can be a value of @ref FLASH_MemType_TypeDef
  * @retval None
  */
void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType)
{
  /* Check parameter */
  assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
  
  /* Lock memory */
  FLASH->IAPSR &= (uint8_t)FLASH_MemType;
}

/**
  * @brief  DeInitializes the FLASH registers to their default reset values.
  * @param  None
  * @retval None
  */
void FLASH_DeInit(void)
{
  FLASH->CR1 = FLASH_CR1_RESET_VALUE;
  FLASH->CR2 = FLASH_CR2_RESET_VALUE;
  FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
  FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_DUL);
  FLASH->IAPSR &= (uint8_t)(~FLASH_IAPSR_PUL);
  (void) FLASH->IAPSR; /* Reading of this register causes the clearing of status flags */
}

/**
  * @brief  Enables or Disables the Flash interrupt mode
  * @param  NewState : The new state of the flash interrupt mode
  *         This parameter can be a value of @ref FunctionalState enumeration.
  * @retval None
  */
void FLASH_ITConfig(FunctionalState NewState)
{
  /* Check parameter */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if(NewState != DISABLE)
  {
    FLASH->CR1 |= FLASH_CR1_IE; /* Enables the interrupt sources */
  }
  else
  {
    FLASH->CR1 &= (uint8_t)(~FLASH_CR1_IE); /* Disables the interrupt sources */
  }
}

/**
  * @brief  Erases one byte in the program or data EEPROM memory
  * @note   PointerAttr define is declared in the stm8s.h file to select if 
  *         the pointer will be declared as near (2 bytes) or far (3 bytes).
  * @param  Address : Address of the byte to erase
  * @retval None
  */
void FLASH_EraseByte(uint32_t Address)
{
  /* Check parameter */
  assert_param(IS_FLASH_ADDRESS_OK(Address));
  
  /* Erase byte */
  *(PointerAttr uint8_t*) (MemoryAddressCast)Address = FLASH_CLEAR_BYTE; 
}

/**
  * @brief  Programs one byte in program or data EEPROM memory
  * @note   PointerAttr define is declared in the stm8s.h file to select if 
  *         the pointer will be declared as near (2 bytes) or far (3 bytes).
  * @param  Address : Address where the byte will be programmed
  * @param  Data : Value to be programmed
  * @retval None
  */
void FLASH_ProgramByte(uint32_t Address, uint8_t Data)
{
  /* Check parameters */
  assert_param(IS_FLASH_ADDRESS_OK(Address));
  *(PointerAttr uint8_t*) (MemoryAddressCast)Address = Data;
}

/**
  * @brief  Reads any byte from flash memory
  * @note   PointerAttr define is declared in the stm8s.h file to select if 
  *         the pointer will be declared as near (2 bytes) or far (3 bytes).
  * @param  Address : Address to read
  * @retval Value of the byte
  */
uint8_t FLASH_ReadByte(uint32_t Address)
{
  /* Check parameter */
  assert_param(IS_FLASH_ADDRESS_OK(Address));
  
  /* Read byte */
  return(*(PointerAttr uint8_t *) (MemoryAddressCast)Address); 
}

/**
  * @brief  Programs one word (4 bytes) in program or data EEPROM memory
  * @note   PointerAttr define is declared in the stm8s.h file to select if 
  *         the pointer will be declared as near (2 bytes) or far (3 bytes).
  * @param  Address : The address where the data will be programmed
  * @param  Data : Value to be programmed
  * @retval None
  */
IN_RAM(void FLASH_ProgramWord(uint32_t Address, uint32_t Data))
{
  /* Check parameters */
  assert_param(IS_FLASH_ADDRESS_OK(Address));
  
  /* Enable Word Write Once */
  FLASH->CR2 |= FLASH_CR2_WPRG;
  FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NWPRG);
  
  /* Write one byte - from lowest address*/
  *((PointerAttr uint8_t*)(MemoryAddressCast)Address)       = *((uint8_t*)(&Data));
  /* Write one byte*/
  *(((PointerAttr uint8_t*)(MemoryAddressCast)Address) + 1) = *((uint8_t*)(&Data)+1); 
  /* Write one byte*/    
  *(((PointerAttr uint8_t*)(MemoryAddressCast)Address) + 2) = *((uint8_t*)(&Data)+2); 
  /* Write one byte - from higher address*/
  *(((PointerAttr uint8_t*)(MemoryAddressCast)Address) + 3) = *((uint8_t*)(&Data)+3); 
}

/**
  * @brief  Programs option byte
  * @param  Address : option byte address to program
  * @param  Data : Value to write
  * @retval None
  */
void FLASH_ProgramOptionByte(uint16_t Address, uint8_t Data)
{
  /* Check parameter */
  assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
  
  /* Enable write access to option bytes */
  FLASH->CR2 |= FLASH_CR2_OPT;
  FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
  
  /* check if the option byte to program is ROP*/
  if(Address == 0x4800)
  {
    /* Program option byte*/
    *((NEAR uint8_t*)Address) = Data;
  }
  else
  {
    /* Program option byte and his complement */
    *((NEAR uint8_t*)Address) = Data;
    *((NEAR uint8_t*)((uint16_t)(Address + 1))) = (uint8_t)(~Data);
  }
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
  
  /* Disable write access to option bytes */
  FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
  FLASH->NCR2 |= FLASH_NCR2_NOPT;
}

/**
  * @brief  Erases option byte
  * @param  Address : Option byte address to erase
  * @retval None
  */
void FLASH_EraseOptionByte(uint16_t Address)
{
  /* Check parameter */
  assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
  
  /* Enable write access to option bytes */
  FLASH->CR2 |= FLASH_CR2_OPT;
  FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);
  
  /* check if the option byte to erase is ROP */
  if(Address == 0x4800)
  {
    /* Erase option byte */
    *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
  }
  else
  {
    /* Erase option byte and his complement */
    *((NEAR uint8_t*)Address) = FLASH_CLEAR_BYTE;
    *((NEAR uint8_t*)((uint16_t)(Address + (uint16_t)1 ))) = FLASH_SET_BYTE;
  }
  FLASH_WaitForLastOperation(FLASH_MEMTYPE_PROG);
  
  /* Disable write access to option bytes */
  FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
  FLASH->NCR2 |= FLASH_NCR2_NOPT;
}

/**
  * @brief  Reads one option byte
  * @param  Address  option byte address to read.
  * @retval Option byte read value + its complement
  */
uint16_t FLASH_ReadOptionByte(uint16_t Address)
{
  uint8_t value_optbyte, value_optbyte_complement = 0;
  uint16_t res_value = 0;
  
  /* Check parameter */
  assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));
    
  value_optbyte = *((NEAR uint8_t*)Address); /* Read option byte */
  value_optbyte_complement = *(((NEAR uint8_t*)Address) + 1); /* Read option byte complement */
  
  /* Read-out protection option byte */
  if(Address == 0x4800)	 
  {
    res_value =	 value_optbyte;
  }
  else
  {
    if(value_optbyte == (uint8_t)(~value_optbyte_complement))
    {
      res_value = (uint16_t)((uint16_t)value_optbyte << 8);
      res_value = res_value | (uint16_t)value_optbyte_complement;
    }
    else
    {
      res_value = FLASH_OPTIONBYTE_ERROR;
    }
  }
  return(res_value);
}

/**
  * @brief  Select the Flash behaviour in low power mode
  * @param  FLASH_LPMode Low power mode selection
  *         This parameter can be any of the @ref FLASH_LPMode_TypeDef values.
  * @retval None
  */
void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef FLASH_LPMode)
{
  /* Check parameter */
  assert_param(IS_FLASH_LOW_POWER_MODE_OK(FLASH_LPMode));
  
  /* Clears the two bits */
  FLASH->CR1 &= (uint8_t)(~(FLASH_CR1_HALT | FLASH_CR1_AHALT)); 
  
  /* Sets the new mode */
  FLASH->CR1 |= (uint8_t)FLASH_LPMode; 
}

/**
  * @brief  Sets the fixed programming time
  * @param  FLASH_ProgTime Indicates the programming time to be fixed
  *         This parameter can be any of the @ref FLASH_ProgramTime_TypeDef values.
  * @retval None
  */
void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef FLASH_ProgTime)
{
  /* Check parameter */
  assert_param(IS_FLASH_PROGRAM_TIME_OK(FLASH_ProgTime));
  
  FLASH->CR1 &= (uint8_t)(~FLASH_CR1_FIX);
  FLASH->CR1 |= (uint8_t)FLASH_ProgTime;
}

/**
  * @brief  Returns the Flash behaviour type in low power mode
  * @param  None
  * @retval FLASH_LPMode_TypeDef Flash behaviour type in low power mode
  */
FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void)
{
  return((FLASH_LPMode_TypeDef)(FLASH->CR1 & (uint8_t)(FLASH_CR1_HALT | FLASH_CR1_AHALT)));
}

/**
  * @brief  Returns the fixed programming time
  * @param  None
  * @retval FLASH_ProgramTime_TypeDef Fixed programming time value
  */
FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void)
{
  return((FLASH_ProgramTime_TypeDef)(FLASH->CR1 & FLASH_CR1_FIX));
}

/**
  * @brief  Returns the Boot memory size in bytes
  * @param  None
  * @retval Boot memory size in bytes
  */
uint32_t FLASH_GetBootSize(void)
{
  uint32_t temp = 0;
  
  /* Calculates the number of bytes */
  temp = (uint32_t)((uint32_t)FLASH->FPR * (uint32_t)512);
  
  /* Correction because size of 127.5 kb doesn't exist */
  if(FLASH->FPR == 0xFF)
  {
    temp += 512;
  }
  
  /* Return value */
  return(temp);
}

/**
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  FLASH_FLAG : Specifies the flag to check.
  *         This parameter can be any of the @ref FLASH_Flag_TypeDef enumeration.
  * @retval FlagStatus : Indicates the state of FLASH_FLAG.
  *         This parameter can be any of the @ref FlagStatus enumeration.
  * @note   This function can clear the EOP, WR_PG_DIS flags in the IAPSR register.
  */
FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG)
{
  FlagStatus status = RESET;
  /* Check parameters */
  assert_param(IS_FLASH_FLAGS_OK(FLASH_FLAG));
  
  /* Check the status of the specified FLASH flag */
  if((FLASH->IAPSR & (uint8_t)FLASH_FLAG) != (uint8_t)RESET)
  {
    status = SET; /* FLASH_FLAG is set */
  }
  else
  {
    status = RESET; /* FLASH_FLAG is reset*/
  }
  
  /* Return the FLASH_FLAG status */
  return status;
}

/**
@code
 All the functions defined below must be executed from RAM exclusively, except
 for the FLASH_WaitForLastOperation function which can be executed from Flash.

 Steps of the execution from RAM differs from one toolchain to another:
 - For Cosmic Compiler:
    1- Define a segment FLASH_CODE by the mean of " #pragma section (FLASH_CODE)".
    This segment is defined in the stm8s_flash.c file.
  2- Uncomment the "#define RAM_EXECUTION  (1)" line in the stm8s.h file,
    or define it in Cosmic compiler preprocessor to enable the FLASH_CODE segment
   definition.
  3- In STVD Select Project\Settings\Linker\Category "input" and in the RAM section
    add the FLASH_CODE segment with "-ic" options.
  4- In main.c file call the _fctcpy() function with first segment character as 
    parameter "_fctcpy('F');" to load the declared moveable code segment
    (FLASH_CODE) in RAM before execution.
  5- By default the _fctcpy function is packaged in the Cosmic machine library,
    so the function prototype "int _fctcopy(char name);" must be added in main.c
    file.

  - For Raisonance Compiler
   1- Use the inram keyword in the function declaration to specify that it can be
    executed from RAM.
    This is done within the stm8s_flash.c file, and it's conditioned by 
    RAM_EXECUTION definition.
   2- Uncomment the "#define RAM_EXECUTION  (1)" line in the stm8s.h file, or 
   define it in Raisonance compiler preprocessor to enable the access for the 
   inram functions.
   3- An inram function code is copied from Flash to RAM by the C startup code. 
   In some applications, the RAM area where the code was initially stored may be
   erased or corrupted, so it may be desirable to perform the copy again. 
   Depending on the application memory model, the memcpy() or fmemcpy() functions
   should be used to perform the copy.
      • In case your project uses the SMALL memory model (code smaller than 64K),
       memcpy()function is recommended to perform the copy
      • In case your project uses the LARGE memory model, functions can be 
      everywhere in the 24-bits address space (not limited to the first 64KB of
      code), In this case, the use of memcpy() function will not be appropriate,
      you need to use the specific fmemcpy() function (which copies objects with
      24-bit addresses).
      - The linker automatically defines 2 symbols for each inram function:
           • __address__functionname is a symbol that holds the Flash address 
           where the given function code is stored.
           • __size__functionname is a symbol that holds the function size in bytes.
     And we already have the function address (which is itself a pointer)
  4- In main.c file these two steps should be performed for each inram function:
     • Import the "__address__functionname" and "__size__functionname" symbols
       as global variables:
         extern int __address__functionname; // Symbol holding the flash address
         extern int __size__functionname;    // Symbol holding the function size
     • In case of SMALL memory model use, Call the memcpy() function to copy the
      inram function to the RAM destination address:
                memcpy(functionname, // RAM destination address
                      (void*)&__address__functionname, // Flash source address
                      (int)&__size__functionname); // Code size of the function
     • In case of LARGE memory model use, call the fmemcpy() function to copy 
     the inram function to the RAM destination address:
                 memcpy(functionname, // RAM destination address
                      (void @far*)&__address__functionname, // Flash source address
                      (int)&__size__functionname); // Code size of the function

 - For IAR Compiler:
    1- Use the __ramfunc keyword in the function declaration to specify that it 
    can be executed from RAM.
    This is done within the stm8s_flash.c file, and it's conditioned by 
    RAM_EXECUTION definition.
    2- Uncomment the "#define RAM_EXECUTION  (1)" line in the stm8s.h file, or 
   define it in IAR compiler preprocessor to enable the access for the 
   __ramfunc functions.

 - Note: 
    1- Ignore the IAR compiler warnings, these warnings don't impact the FLASH Program/Erase
    operations.
    The code performing the Flash Program/erase must be executed from RAM; the variables
    initializations don't necessary require the execution from RAM, only CR2/NCR2 registers 
    configuration and data programing must be executed from RAM.
    2- These warnings depends on IAR compiler: as the code generation is made using many
    runtime library functions to keep code size to a minimum.
    3- It is recommended to use High Speed Optimization with IAR (-Ohs), in order 
    to reduce the runtime library calls in the generated code.



 The FLASH examples given within the STM8S_StdPeriph_Lib package, details all 
 the steps described above.

@endcode
*/

/**
  * @brief
  *******************************************************************************
  *                         Execution from RAM enable
  *******************************************************************************
  *
  * To enable execution from RAM you can either uncomment the following define 
  * in the stm8s.h file or define it in your toolchain compiler preprocessor
  * - #define RAM_EXECUTION  (1) 
  */
  
#if defined (_COSMIC_) && defined (RAM_EXECUTION)
 #pragma section (FLASH_CODE)
#endif  /* _COSMIC_ && RAM_EXECUTION */
/**
  * @brief  Wait for a Flash operation to complete.
  * @note   The call and execution of this function must be done from RAM in case
  *         of Block operation.
  * @param  FLASH_MemType : Memory type
  *         This parameter can be a value of @ref FLASH_MemType_TypeDef
  * @retval FLASH status
  */
IN_RAM(FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType)) 
{
  uint8_t flagstatus = 0x00;
  uint16_t timeout = OPERATION_TIMEOUT;
  
  /* Wait until operation completion or write protection page occurred */
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
  defined(STM8S005) || defined(STM8AF52Ax) || defined(STM8AF62Ax) || defined(STM8AF626x)  
    if(FLASH_MemType == FLASH_MEMTYPE_PROG)
    {
      while((flagstatus == 0x00) && (timeout != 0x00))
      {
        flagstatus = (uint8_t)(FLASH->IAPSR & (uint8_t)(FLASH_IAPSR_EOP |
                                                        FLASH_IAPSR_WR_PG_DIS));
        timeout--;
      }
    }
    else
    {
      while((flagstatus == 0x00) && (timeout != 0x00))
      {
        flagstatus = (uint8_t)(FLASH->IAPSR & (uint8_t)(FLASH_IAPSR_HVOFF |
                                                        FLASH_IAPSR_WR_PG_DIS));
        timeout--;
      }
    }
#else /*STM8S103, STM8S903, STM8AF622x */
  while((flagstatus == 0x00) && (timeout != 0x00))
  {
    flagstatus = (uint8_t)(FLASH->IAPSR & (FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS));
    timeout--;
  }
#endif /* STM8S208, STM8S207, STM8S105, STM8AF52Ax, STM8AF62Ax, STM8AF262x */
  
  if(timeout == 0x00 )
  {
    flagstatus = FLASH_STATUS_TIMEOUT;
  }
  
  return((FLASH_Status_TypeDef)flagstatus);
}

/**
  * @brief  Erases a block in the program or data memory.
  * @note   This function should be executed from RAM.
  * @param  FLASH_MemType :  The type of memory to erase
  * @param  BlockNum : Indicates the block number to erase
  * @retval None.
  */
IN_RAM(void FLASH_EraseBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType))
{
  uint32_t startaddress = 0;
  
#if defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) || \
  defined (STM8S903) || defined (STM8AF626x) || defined (STM8AF622x)
    uint32_t PointerAttr  *pwFlash;
#elif defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF62Ax) || defined (STM8AF52Ax) 
  uint8_t PointerAttr  *pwFlash;
#endif
  
  /* Check parameters */
  assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
  if(FLASH_MemType == FLASH_MEMTYPE_PROG)
  {
    assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
    startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
  }
  else
  {
    assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
    startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
  }
  
  /* Point to the first block address */
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF62Ax) || defined (STM8AF52Ax)
  pwFlash = (PointerAttr uint8_t *)(MemoryAddressCast)(startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE));
#elif defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) || \
  defined (STM8S903) || defined (STM8AF626x) || defined (STM8AF622x)
    pwFlash = (PointerAttr uint32_t *)(MemoryAddressCast)(startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE));
#endif	/* STM8S208, STM8S207 */
  
  /* Enable erase block mode */
  FLASH->CR2 |= FLASH_CR2_ERASE;
  FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NERASE);
  
#if defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) ||  \
  defined (STM8S903) || defined (STM8AF626x) || defined (STM8AF622x)
    *pwFlash = (uint32_t)0;
#elif defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF62Ax) || \
  defined (STM8AF52Ax)
    *pwFlash = (uint8_t)0;
  *(pwFlash + 1) = (uint8_t)0;
  *(pwFlash + 2) = (uint8_t)0;
  *(pwFlash + 3) = (uint8_t)0;    
#endif
}

/**
  * @brief  Programs a memory block
  * @note   This function should be executed from RAM.
  * @param  FLASH_MemType : The type of memory to program
  * @param  BlockNum : The block number
  * @param  FLASH_ProgMode : The programming mode.
  * @param  Buffer : Pointer to buffer containing source data.
  * @retval None.
  */
IN_RAM(void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, 
                        FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer))
{
  uint16_t Count = 0;
  uint32_t startaddress = 0;
  
  /* Check parameters */
  assert_param(IS_MEMORY_TYPE_OK(FLASH_MemType));
  assert_param(IS_FLASH_PROGRAM_MODE_OK(FLASH_ProgMode));
  if(FLASH_MemType == FLASH_MEMTYPE_PROG)
  {
    assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
    startaddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
  }
  else
  {
    assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
    startaddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
  }
  
  /* Point to the first block address */
  startaddress = startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE);
  
  /* Selection of Standard or Fast programming mode */
  if(FLASH_ProgMode == FLASH_PROGRAMMODE_STANDARD)
  {
    /* Standard programming mode */ /*No need in standard mode */
    FLASH->CR2 |= FLASH_CR2_PRG;
    FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NPRG);
  }
  else
  {
    /* Fast programming mode */
    FLASH->CR2 |= FLASH_CR2_FPRG;
    FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NFPRG);
  }
  
  /* Copy data bytes from RAM to FLASH memory */
  for(Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
  {
    *((PointerAttr uint8_t*) (MemoryAddressCast)startaddress + Count) = ((uint8_t)(Buffer[Count]));
  }
}

#if defined (_COSMIC_) && defined (RAM_EXECUTION)
 /* End of FLASH_CODE section */
 #pragma section ()
#endif /* _COSMIC_ && RAM_EXECUTION */


/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
