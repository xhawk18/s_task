/** @page TIM4_TimeBase  Use the TIM4 peripheral to generate 1 ms time base using 
 *   TIM4 update interrupt

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    TIM4/TIM4_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the TIM4 Time Base Example.
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

  This example provides a short description of how to use the TIM4 peripheral 
  to generate time base equal to 1 ms. In this example a "Delay" function is 
  implemented based on the TIM4 time base.
  The four LEDs LD1..LD4 toggle with a timing defined by the Delay function.

  The TIM4CLK frequency is 16MHz, the Prescaler is 128 so the TIM4 counter 
  clock is 125 KHz.
  The TIM4 is running at TIM4 frequency = TIM4 counter clock / (TIM4_PERIOD + 1   
                                        = 125000 / 125 = 1 KHz.
  So the TIM4 generates an Update Interrupt each 1 / 1000 = 1 ms.
  


  @par Directory contents

  - TIM4\TIM4_TimeBase\main.c            Main file containing the "main" function
  - TIM4\TIM4_TimeBase\stm8s_conf.h      Library Configuration file
  - TIM4\TIM4_TimeBase\stm8s_it.c        Interrupt routines source
  - TIM4\TIM4_TimeBase\stm8s_it.h        Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density devices and
    STM8S103x Low density devices.
      
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Four leds LD1..LD4


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
