/** @page FLASH_ByteReadWriteOperation Read, Write, Erase byte
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    FLASH/FLASH_ByteReadWriteOperation/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the description of the FLASH byte read write 
  *          operation example.
  ******************************************************************************
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
  @endverbatim

  @par Example description

  This example provides a short description on how to use FLASH Read/Write/Erase
  operations on byte:
  - Read a byte at a specified address (0x40A5).
  - Write its complement value at address +1 (0x40A6).
  - Check programmed byte (using read function)
  - Erase 2 bytes at address & address+1
  - Check 2 bytes are erased (value must be equal to 0x00).


  @par Directory contents

  - FLASH\FLASH_ByteReadWriteOperation\main.c          Main file containing the "main" function
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_conf.h    Library Configuration file
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_it.c      Interrupt routines source (not used in this example)
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_it.h      Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

 
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - The data read from the data memory is compared with the expected values.
    The result of this comparison is stored in the "OperationStatus" variable.

  @note
  - High-Density STM8A devices are the STM8AF52xx STM8AF6269/8x/Ax,
    STM8AF51xx, and STM8AF6169/7x/8x/9x/Ax microcontrollers where the Flash memory
    density ranges between 32 to 128 Kbytes
  - Medium-Density STM8A devices are the STM8AF622x/4x, STM8AF6266/68,
    STM8AF612x/4x, and STM8AF6166/68 microcontrollers where the Flash memory 
    density ranges between 8 to 32 Kbytes
  - High-Density STM8S devices are the STM8S207xx, STM8S007 and STM8S208xx microcontrollers
    where the Flash memory density ranges between 32 to 128 Kbytes.
  - Medium-Density STM8S devices are the STM8S105x and STM8S005 microcontrollers
    where the Flash memory density ranges between 16 to 32-Kbytes.
  - Low-Density STM8S devices are the STM8S103xx, STM8S003 and STM8S903xx microcontrollers
    where the Flash density is 8 Kbytes.
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
