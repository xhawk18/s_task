/** @page WWDG_Example WWDG refreshing, WWDG Reset

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    WWDG/WWDG_Example/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the WWDG Rearm Reset Example.
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

  This example shows how to refresh the WWDG counter. The WWDG timeout is set to
  251.9 ms.
  
  As AllowedRefresh variable is set to ENABLE the WWDG counter is refreshed 
  preventing a WWDG reset and LED2 is toggling.
  When user press key button, an EXTI event is triggered and the corresponding
  ISR is served setting AllowedRefresh variable to DISABLE. So no WWDG counter
  refresh is performed and when WWDG counter reaches 63 an MCU reset will occur.

  When user press joystick down button, an EXTI event is triggered and the 
  corresponding ISR is served setting NonAlowedRefresh variable to ENABLE. 
  So WWDG counter refresh is performed in the non allowed window and as a result
  an MCU reset will occur.

  If a WWDG reset is generated, after system startup LED1 toggles otherwise it
  turns off.

  

  @par Directory contents

  - WWDG\WWDG_Example\main.c            Main file containing the "main" function
  - WWDG\WWDG_Example\stm8s_conf.h      Library Configuration file
  - WWDG\WWDG_Example\stm8s_it.c        Interrupt routines source 
  - WWDG\WWDG_Example\stm8s_it.h        Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density Low density
    devices.
      
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - leds LD1, LD2, Key and joystick down push buttons are used.


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - LED 2 is toggling, WWDG counter is refreshed in allowed window so no reset.
  - When you press key button, WWDG counter is never refreshed and when it
    reaches 63, LED1 toggles indicating a WWDG reset.
  - When you press joystick down button, WWDG counter is refreshed in the non
    allowed window so LED1 toggles indicating a WWDG reset.

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
