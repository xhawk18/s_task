/** @page I2C_EEPROM Write & Read data in an I2C EEPROM memory
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    I2C/I2C_EEPROM/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the I2C and M24C64 EEPROM communication example.
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

This firmware provides a basic example of how to use the I2C firmware library and
an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the
example is interfacing with M24C64 EEPROM)

I2C peripheral is configured in Master transmitter during write operation and in
Master receiver during read operation from I2C EEPROM. 

The speed is set to 200kHz and can be configured by modifying the related define
in stm8s_eval_i2c_ee.h file.

For M24C64 devices all the memory is accessible through the two-bytes 
addressing mode and need to define block addresses. In this case, only the physical 
address has to be defined (according to the address pins (E0,E1 and E2) connection).
This address is defined in stm8s_eval_i2c_ee.h (default is 0xA0: E0, E1 and E2 
tied to ground). 
The EEPROM addresses where the program start the write and the read operations 
is defined in the main.c file. 

First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the
written data is read. The written and the read buffers data are then compared.
Following the read operation, the program waits that the EEPROM reverts to its 
Standby state. A second write operation is, then, performed and this time, Tx2_Buffer
is written to EEPROM_WriteAddress2, which represents the address just after the last 
written one in the first write. After completion of the second write operation, the 
written data is read. The contents of the written and the read buffers are compared.

This example provides the possibility to use the LCD available on the STM8/128-EVAL
board for messages display (transfer status: Ongoing, PASSED, FAILED).
To enable this option uncomment the define ENABLE_LCD_MSG_DISPLAY in the main.c
file.


  @par Directory contents

  - I2C\I2C_EEPROM\main.c                    Main file containing the "main" function
  - I2C\I2C_EEPROM\stm8s_conf.h              Library Configuration file
  - I2C\I2C_EEPROM\stm8s_it.c                Interrupt routines source
  - I2C\I2C_EEPROM\stm8s_it.h                Interrupt routines declaration


 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
      
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - This example uses the I2C EEPROM already available on STM8/128-EVAL board.
     - Before Running the example make sure that JP8 is fitted.
     

  @par How to use it ?


  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8s_eval_i2c_ee.c  (under Utilities\STM8S_EVAL\Common)   
    - stm8s_eval_lcd.c  (under Utilities\STM8S_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  
  
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
