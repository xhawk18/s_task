/** @page ITC_SoftwarePriority Changing software priority on two external interrupt lines

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    ITC/ITC_SoftwarePriority/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the description of ITC software priority 
  *          example.
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

  This example shows how to change the software priority of two external 
  interrupt lines to allow nested interrupt management.
  In this example, we set the Key IRQ software priority level to 1 
  and set the joystick Down IRQ software priority level to 2.
  Thus Key button IRQ software priority is higher than joystick Down one and 
  pressing Key button interrupts the execution of the joystick Down Interrupt 
  Subroutine.
  If the Key IRQ and joystick Down IRQ had the same software priority, neither
  Key IRQ could interrupt joystick Down IRQ nor joystick Down could interrupt 
  Key IRQ.


  @par Directory contents

  - ITC\ITC_SoftwarePriority\main.c          Main file containing the "main" function
  - ITC\ITC_SoftwarePriority\stm8s_conf.h    Library Configuration file
  - ITC\ITC_SoftwarePriority\stm8s_it.c      Interrupt routines source
  - ITC\ITC_SoftwarePriority\stm8s_it.h      Interrupt routines declaration


 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Two leds LED1 to LED2, Key push button and Joystick Down button are used.
     
  - STM8/128-EVAL Set-up
      - LED1 & LED2
      - Joystick DOWN
      - Key button 
      
      
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - The LED1 and LED2 are ON per default
  - When the Joystick Down button is pressed, the LED1 toggles and LED2 is OFF
  - When the Key button is pressed, the LED2 toggles and LED1 is OFF
  - Play with the Button and the Joystick Down in order to see which one has
    the highest priority:
     - press and maintain the Joystick Down Button -> LED1 toggles, LED2 OFF
     - Continue pressing the Joystick Down Button and press the Key button 
     -> LED2 toggles instead, LED1 OFF (Key has the priority)
     If you do the reverse, you can see that the KEY button is not interrupted 
     by the Joystick button as configured by hardware.

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
