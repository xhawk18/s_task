/** @page TIM1_ComplementarySignals_DeadTime_Break_Lock Generating 6 complementary 
 *        PWM signals with dead time insertion, break capability and lock option using TIM1

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    TIM1/TIM1_ComplementarySignals_DeadTime_Break_Lock/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the TIM1 Complementary Signals, Dead Time, Break and 
  *          Lock Example.
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

  This example shows how to configure the TIM1 peripheral to generate three
  complementary TIM1 signals, to insert a defined dead time value, to use the
  break feature and to lock the desired parameters.

  TIM1CLK is fixed to 2 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 2 MHz. (By default, fTimer1= fMaster/8 )
  TIM1 frequency is defined as follow:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 30.52 Hz.
  
  The Three Duty cycles are computed as the following description: 
  
  TIM1 Channel1 duty cycle = TIM1_CCR1 / (TIM1_Period + 1) = 50% 
  TIM1 Channel1N duty cycle = (TIM1_Period - TIM1_CCR1) / (TIM1_Period + 1) = 50%
  
  TIM1 Channel2 duty cycle = TIM1_CCR2 / (TIM1_Period + 1) = 25%
  TIM1 Channel2N duty cycle = (TIM1_Period - TIM1_CCR2) / (TIM1_Period + 1) = 75% 
  
  TIM1 Channel3 duty cycle = TIM1_CCR3 / TIM1_Period = 12.5% 
  TIM1 Channel3N duty cycle = (TIM1_Period - TIM1_CCR3) / (TIM1_Period + 1) = 87.5%
  
  A dead time of 59 us is inserted between the different complementary signals, 
  and the Lock level 1 is selected.
  The break Polarity is used at High level.

  DTG[7:0]= 117 = (01110101)binary  => DTG[7:5]=0xx => Dead Time = DTG[7:0]xTIM1Clk
  TIM1Clk =  500 ns (2MHz) . So TIM1 Dead time = 58.5 us
  
  The TIM1 waveform can be displayed using an oscilloscope. 
  
  @note  Connect the TIM1 break TIM1_BKIN pin (PE.3) to the GND.
         To generate a break event, switch this pin level from 0V to 3.3V.


  @par Directory contents

  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\main.c          Main file containing the "main" function
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_conf.h    Library Configuration file
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_it.c      Interrupt routines source
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_it.h      Interrupt routines declaration
  

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


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Connect TIM1_BKIN pin(PE.3) to the GND.
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
