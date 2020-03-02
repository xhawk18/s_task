/** @page EXTI_InterruptPriority Reading buttons state using external interrupts
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    EXTI/EXTI_InterruptPriority/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the description of EXTI priority example.
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

  This example provides a short description of how to use the EXTI peripheral:
  - Configuration of External Interrupt / Falling edge and TLI
  - LEDs are toggling at each interrupt detection


  @par Directory contents

  - EXTI\EXTI_InterruptPriority\main.c             Main file containing the "main" function
  - EXTI\EXTI_InterruptPriority\main.h             Main file containing the "main" header function
  - EXTI\EXTI_InterruptPriority\stm8s_conf.h       Library Configuration file
  - EXTI\EXTI_InterruptPriority\stm8s_it.c         Interrupt routines source
  - EXTI\EXTI_InterruptPriority\stm8s_it.h         Interrupt routines declaration
  
  
 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Four leds LD1 to LD4 are used.
     - Joystick LEFT, RIGHT, UP, DOWN and SEL buttons are used.


  @par How to use it ?


  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - The LD1, LD2, LD3 and LD4 are ON per default
  - Joystick LEFT  pressed: toggle LED1
  - Joystick RIGHT pressed: toggle LED2
  - Joystick UP    pressed: toggle LED3
  - Joystick DOWN  pressed: toggle LED4
  - Joystick SEL   pressed: toggle all LEDs

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
