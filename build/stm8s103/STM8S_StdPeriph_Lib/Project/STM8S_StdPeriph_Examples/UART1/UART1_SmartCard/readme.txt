/** @page UART1_SmartCard  UART1 Smart Card mode

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    UART1/UART1_SmartCard/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the UART1 Smart card Example.
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

  This example provides a description of how  to use the UART1 in Smart Card mode. 
  The example gives only the possibility to read the ATR and decode it into a 
  predefined buffer.

  First, the code  waits for a card insertion. If the card is detected, a reset 
  signal is applied to the card through its reset pin.

  As response to this reset, the card transmits the ATR which is stored in a 
  predefined buffer.
  Once the ATR is received, it is decoded and stored in a specific structure (SC_A2R)
  and the card protocol type is stored in a variable (ATRDecodeStatus).

  The UART1 Smartcard mode supports asynchronous protocol Smartcards as defined 
  in the ISO 7816-3(Class A) standard, please refer to the ISO 7816-3 specification
  or to the application note AN2646 (Smartcard interface with the STM8S20x) for 
  more details.

  The UART1 is configured as follows:
    - Word Length = 9 Bits
    - 1.5 Stop Bit
    - Even parity
    - BaudRate = 10752 baud
    - Transmit and receive enabled
    - UART1 Clock enabled
    - UART1 CPOL: Clock is active low
    - UART1 CPHA: Data is captured on the second edge 
    - UART1 LastBit: The clock pulse of the last data bit is not output to 
                     the SCLK pin (Last bit Disabled).

	
  @par Directory contents

  - UART1\UART1_SmartCard\main.c             Main file containing the "main" function
  - UART1\UART1_SmartCard\stm8s_conf.h       Library Configuration file
  - UART1\UART1_SmartCard\stm8s_it.c         Interrupt routines source
  - UART1\UART1_SmartCard\stm8s_it.h         Interrupt routines declaration
  

  @par Hardware and Software environment
  
  - This example runs on STM8S High density and Low density devices and on STM8A
    High density devices.    
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
    - Plug a smart card (ISO7816-3 T=0 compatible) into the dedicated smart card
      connector CN12.  

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Check the card protocol type variable(ATRDecodeStatus)

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
