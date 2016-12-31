/** @page AWU_ActiveHaltMode Using the AWU to wake-up the CPU from Active Halt mode
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    AWU/AWU_ActiveHaltMode/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the main function for AWU Active Halt Mode example.
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

  This example provides a short description of how to use the AWU peripheral:
  Pressing Key button will enter MCU in Halt mode.
  The MCU will then exit from halt mode thanks to the AWU. The wake up time depends 
  on the AWU peripheral configuration. 
  MCU state will be monitored by LCD display and LED blinking.


  @par Directory contents

  - AWU\AWU_ActiveHaltMode\main.c           Main file containing the "main" function
  - AWU\AWU_ActiveHaltMode\stm8s_conf.h     Library Configuration file
  - AWU\AWU_ActiveHaltMode\stm8s_it.c       Interrupt routines source (not used in this example)
  - AWU\AWU_ActiveHaltMode\stm8s_it.h       Interrupt routines declaration
  
  
  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  @note: to run this example with STM8S Low Density devices, TIM1 should be used 
  instead of TIM3 for LSI calibration.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Key push button
     - LD1..LD4
     - Mono LCD

  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain
  - Add the required file:
    - stm8s_eval_lcd.c  (under Utilities\STM8S_EVAL\Common)     
  - Rebuild all files and load your image into target memory
  - Run the example
  - The 4 LEDs (LD1 to LD4) blink continuously and slowly
  - If you press the Key button on the board, the LEDs will blink faster for
    a few seconds and then will be switched off.
  - At this stage, the MCU is in Active Halt mode.
  - After few seconds (12s), the MCU will exit from halt mode thanks to the AWU.
  - The 4 LEDs will continue to blink slowly.

  
  @par Hint
  
  - Before using the LSI clock you have to configure the option bytes in order 
    to enable the "LSI_EN".
  - To do so :
    - with STVD (menu: Debug Instrument -> MCU configuration -> Options -> LSI_EN = Enabled )
    - with RIDE (edit the "STM8_OB.asm" file to configure the OPT3 & NOPT3 option bytes)
    - with EWSTM8 (menu: ST-LINK -> Option bytes -> LSI_EN = Enabled)       
  
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