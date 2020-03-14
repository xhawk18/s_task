/** @page SPI_FullDuplexUART1 Full duplex master/slave communication with UART1.
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    SPI/SPI_FullDuplexUART1/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the SPI Full Duplex with UART Example.
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

  This example provides a short description of how to make a master/slave full duplex 
  communication between the SPI and the UART1, the last is configured in master
  synchronous mode.
  
  The UART1 sends data from TxBuffer1 buffer to SPI using TXE flag.
  Data received using RXNE flag by SPI is stored in RxBuffer2 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus1" 
  variable.
 
  In the same Time, the SPI sends data from TxBuffer2 buffer to UART1 using TXE flag.
  Data received using RXNE flag by UART1 is stored in RxBuffer1 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus2" 
  variable. 

  @par Directory contents

  - SPI\SPI_FullDuplexUART1\main.c           Main file containing the "main" function
  - SPI\SPI_FullDuplexUART1\stm8s_conf.h     Library Configuration file
  - SPI\SPI_FullDuplexUART1\stm8s_it.c       Interrupt routines source
  - SPI\SPI_FullDuplexUART1\stm8s_it.h       Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density,Low density devices.
  - For medium density devices, UART1 should be replaced by UART2
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
      - Connect UART1 and SPI pins as follows:
         - UART1 TX (PA5) to SPI MOSI (PC6)
         - UART1 RX (PA4) to SPI MISO (PC7)
         - UART1 CLK (PA6) to SPI SCK  (PC5)


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
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
