/** @page TIM1_7PWM_Output Generate 7 PWM signals with 4 different duty cycles

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    TIM1/TIM1_7PWM_Output/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the TIM1 7 PWM Output Example.
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

  This example shows how to configure the TIM1 peripheral to generate 7 PWM signals
  with 4 different duty cycles.

  TIM1CLK is fixed to 2 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 2 MHz. (By default, fTimer1= fMaster/8 )
  
  TIM1 frequency is defined as follows:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 488.28 Hz.
  
  The TIM1 CCR1 register value is equal to 0x7FF, so the TIM1 Channel 1 and TIM1 
  Channel 1N generate a PWM signal with a frequency equal to 488.28Hz 
  and a duty cycle equal to:
  TIM1 Channel1 duty cycle = TIM1_CCR1 /( TIM1_Period + 1) = 50%.

  The TIM1 CCR2 register value is equal to 0x5FF, so the TIM1 Channel 2 and TIM1
  Channel 2N generate a PWM signal with a frequency equal to 488.28 Hz
  and a duty cycle equal to:
  TIM1 Channel2 duty cycle = TIM1_CCR2 / ( TIM1_Period + 1)= 37.5%.

  The TIM1 CCR3 register value is equal to 0x3FF, so the TIM1 Channel 3 and TIM1 
  Channel 3N generate a PWM signal with a frequency equal to 488.28 Hz 
  and a duty cycle equal to:
  TIM1 Channel3 duty cycle = TIM1_CCR3 / ( TIM1_Period + 1) = 25%.

  The TIM1 CCR4 register value is equal to 0x1FF, so the TIM1 Channel 4 
  generate a PWM signal with a frequency equal to 488.28 Hz 
  and a duty cycle equal to:
  TIM1 Channel4 duty cycle = TIM1_CCR4 / ( TIM1_Period + 1) = 12.5%.

  The TIM1 waveform can be displayed using an oscilloscope.


  @par Directory contents

  - TIM1\TIM1_7PWM_Output\main.c          Main file containing the "main" function
  - TIM1\TIM1_7PWM_Output\stm8s_conf.h    Library Configuration file
  - TIM1\TIM1_7PWM_Output\stm8s_it.c      Interrupt routines source
  - TIM1\TIM1_7PWM_Output\stm8s_it.h      Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - Connect TIM1 pins to an oscilloscope as follows:
     - TIM1_CH1  pin (PC.1)  
     - TIM1_CH1N pin (PH.7)  
     - TIM1_CH2  pin (PC.2)  
     - TIM1_CH2N pin (PH.6)  
     - TIM1_CH3  pin (PC.3)  
     - TIM1_CH3N pin (PH.5)  
     - TIM1_CH4 pin  (PC.4)


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Connect the TIM1 pins to an oscilloscope 

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
