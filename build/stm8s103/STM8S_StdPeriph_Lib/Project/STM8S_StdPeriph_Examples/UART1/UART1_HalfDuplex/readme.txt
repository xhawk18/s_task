 /** @page UART1_HalfDuplex UART1 in Half-Duplex mode

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    UART1/UART1_HalfDuplex/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the UART1 in Half-Duplex mode Example.
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

  This example provides a basic Half-Duplex communication between UART1 and UART3
  using flags.

  When running the transmitter, the UART1 sends data from TxBuffer1 buffer to UART3
  using TXE flag.
  Data received using RXNE flag by UART3 is stored in RxBuffer2 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus1" 
  variable.
 
  When running the receiver, the UART3 sends data from TxBuffer2 buffer to UART1
  using TXE flag.
  Data received using RXNE flag by UART1 is stored in RxBuffer1 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus2" 
  variable. 

  The UART3 and UART1 are configured as follows:
   - BaudRate = 230400 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - None parity
   - Receive and transmit enabled
   - UART1 Clock disabled


  @par Directory contents

  - UART1\UART1_HalfDuplex\main.c          Main file containing the "main" function
  - UART1\UART1_HalfDuplex\stm8s_conf.h    Library Configuration file
  - UART1\UART1_HalfDuplex\stm8s_it.c      Interrupt routines source
  - UART1\UART1_HalfDuplex\stm8s_it.h      Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density devices only.
      
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - To run the transmitter: Connect the UART1_TX (PA5) on the UART3_RX(PD6),
     uncomment the line "#define transmitter" and comment the line "#define receiver"
     in main.c file 
 
  - To run the receiver: Connect the UART1_TX (PA5)on the UART3_TX (PD5), 
     uncomment the line "#define receiver" and comment the line "#define transmitter"
     in main.c file

 
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Communication result in Receiver/ Transmitter modes can be checked respectively 
    via TransferStatus2 and TransferStatus1 variables.
  
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
