/** @page CLK_ClockSecuritySystem CSS (Clock security system) capability
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    CLK/CLK_ClockSecuritySystem/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the main function for Clock Security System example.
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

  This example demonstrates the CSS (Clock Security System) capability. 
  The MCU clock system is configured as follows:
	- HSI Prescaler = CLK_PRESCALER_HSIDIV8;
	- Switch Mode   = CLK_SWITCHMODE_AUTO
	- New Clock     = CLK_SOURCE_HSE;
	- Current Clock State  = CLK_CURRENTCLOCKSTATE_DISABLE;
	- CSS = ENABLE 
  
  - When detecting the change of clock from HSE to HSI/8 (auxiliary), 
    LEDs blinking frequency becomes slower.


  @par Directory contents

  - CLK\CLK_ClockSecuritySystem\main.c          Main file containing the "main" function
  - CLK\CLK_ClockSecuritySystem\stm8s_conf.h    Library Configuration file
  - CLK\CLK_ClockSecuritySystem\stm8s_it.c      Interrupt routines source
  - CLK\CLK_ClockSecuritySystem\stm8s_it.h      Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - LD1..LD4


  @par How to use it ?


  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Observe the initial device frequency setting (HSE) on LED1, 2, 3, 4 
    of the Evaluation board (fast blinking frequency).
  - Disconnect the crystal. The system clock will then switch to HSI backup 
    clock as the CSS is enabled.
  - The CPU is still processing operations but the frequency has changed to HSI/8 
  (CSS default clock): LED1, 2, 3, 4 are toggling at another frequency (Slower). 
  - The clock switching can be monitored by putting a scope on PE0 (CCO pin).

  @par Hint
  
  - Before using the HSE clock make sure  that the "Flash_Wait_States" is set to 1. 
    - with STVD (menu: Debug Instrument -> MCU configuration -> Options -> Flash_Wait_States: 1)
    - with RIDE (edit the "STM8_OB.asm" file to configure OPT7 & NOPT7 option bytes)
    - with EWSTM8 (menu: ST-LINK -> Option bytes ->Flash_Wait_States: 1)
    
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
