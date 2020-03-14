/** @page IWDG_Example IWDG example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    IWDG/IWDG_Example/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the IWDG example.
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

 @par Example Description 

 This example shows how to update at regular period the IWDG reload counter and 
 how to simulate a software fault generating an MCU IWDG reset on expiry of a 
 programmed time period.

 The IWDG timeout is set to 250 ms (the timeout may varies due to LSI frequency 
 dispersion).

 First, the TIM3 timer is configured to measure the LSI frequency as the 
 LSI is internally connected to it, in order to adjust the IWDG clock.
 The IWDG reload counter is configured to obtain 247ms according to the 
 measured LSI frequency.

 The Key button is used to generate an interrupt that will simulate a software 
 failure by pressing it.
 In the ISR, a trap instruction is executed generating a TRAP interrupt containing
 an infinite loop and preventing to return to main program (the IWDG reload counter 
 is not refreshed).
 As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
 If the IWDG reset is generated, after the system resumes from reset, LED1 turns on.

 If the Key button is not pressed, the IWDG counter is refreshed in an infinite loop
 and there is no IWDG reset.


  @par Directory contents 

  - IWDG/IWDG_Example/stm8s_conf.h    Library Configuration file
  - IWDG/IWDG_Example/stm8s_it.c      Interrupt handlers
  - IWDG/IWDG_Example/stm8s_it.h      Interrupt handlers header file
  - IWDG/IWDG_Example/main.c          Main program
  
         
  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density Low density
    devices.
    
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
    - LED1 
    - KEY push button 
      
  @par How to use it ? 

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - If you press the key button, a Reset occurs and LED1 turn on.

  @par Hint
   - Before using the LSI clock you have to configure the option bytes in order 
    to enable the "LSI_EN".
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
