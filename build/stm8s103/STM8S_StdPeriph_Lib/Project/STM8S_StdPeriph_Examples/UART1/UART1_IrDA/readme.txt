/** @page UART1_IrDA Use the UART1 in a basic IrDA communication receive/transmit mode.

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    UART1/UART1_IrDA/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the UART1 IrDA Example.
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

  The example provides two IrDA programs: transmitter / receiver and requires two
  boards to be able to run the full demonstration:
   - one board will act as IrDA transmitter
   - one board will act as IrDA receiver

  On the Transmitter board: Five pins, configured in input floating mode, are 
  used to select the byte to be sent at each pin state change
  The byte can be:
   - 0x00 if no key pressed
   - 0x01 if Joystick Sel pressed
   - 0x02 if Joystick Down pressed
   - 0x03 if Joystick Left pressed
   - 0x04 if Joystick Right pressed
   - 0x05 if Joystick Up pressed

   On the Receiver board: Four leds are used to show which byte is received. 
  - LED4 toggle when 0x05 is received (Joystick Up pressed in Transmitter board)  
  - LED3 toggle when 0x02 is received (Joystick Down pressed in Transmitter board)  
  - LED2 toggle when 0x03 is received (Joystick Left pressed in Transmitter board) 
  - LED1 toggle when 0x04 is received (Joystick Right pressed in Transmitter board)  
  - LED1 to LED4 toggle when 0x01 is received (Joystick Sel pressed in Transmitter board)         

  USART configured as follow:
   - BaudRate = 9600 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Receive and transmit enabled

  @par Directory contents

  - UART1\UART1_IrDA\Receive\main.c            Main file containing the "main" function
  - UART1\UART1_IrDA\Receive\stm8s_conf.h      Library Configuration file
  - UART1\UART1_IrDA\Receive\stm8s_it.c        Interrupt routines source (not used in this example)
  - UART1\UART1_IrDA\Receive\stm8s_it.h        Interrupt routines declaration
  

  @par Hardware and Software environment
- This example runs on STM8S High density and Low density devices and on STM8A
    High density devices.    
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - IrDA tranceiver (U14) is used, make sure that JP10 jumper must be fitted.
     - Four leds LD1 to LD4 are used. 
     - Joystick


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
