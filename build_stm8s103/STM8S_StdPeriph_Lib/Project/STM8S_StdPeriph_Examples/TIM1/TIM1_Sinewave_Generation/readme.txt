/** @page TIM1_Sinewave_Generation Generate a sine wave in the range of 45Hz to 
 *        65Hz with the resolution of 0.1Hz

  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    TIM1/TIM1_Sinewave_Generation/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the TIM1 Sine Wave generation  Example.
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

  This example shows how to use TIM1 to get the variation in sine wave frequency from 
  65Hz to 45Hz. 
    
  TIM1CLK is fixed to 4 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 4 MHz. 
  
  The TIM1 Channel 1 is configured as PWM Output Mode 1.
  
  To generate a sine wave of frequency 65.3595, you have to uncomment "#define FREQ_65_3595"
  (User can define only specific frequencies, which are commented out in "main.h"). 
  According to frequency selected by define directive, five parameters will be defined. 
  These parameters will make the corresponding sine wave frequency.
  
  The TIM1_CC1 must be connected to a "Low Pass" filter (Rfilter =1.8Kohm, Cfilter= 0.47uF)

 @code  
  TIM1_CC1 |-><-----[Rfilter=1.8Kohm]----+-------------> to the oscilloscope
                                         |
                  [Cfilter=0.47uF]
                         |
                      ///// GND
 @endcode 

  @par Directory contents

  - TIM1\TIM1_Sinewave_Generation\main.h          Defines macros for all the 
  frequencies in the range of 45Hz to 65Hz with the resolution of 0.1Hz. 
  - TIM1\TIM1_Sinewave_Generation\main.c          Main file containing the "main" function
  - TIM1\TIM1_Sinewave_Generation\stm8s_conf.h    Library Configuration file
  - TIM1\TIM1_Sinewave_Generation\stm8s_it.c      Interrupt routines source
  - TIM1\TIM1_Sinewave_Generation\stm8s_it.h      Interrupt routines declaration
  


 @par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Connect  TIM1_CC1 pin (PC.1) to a "Low Pass" filter (Rfilter =1.8Kohm, Cfilter= 0.47uF).
     - Connect the "Low Pass" filter output to an oscilloscope.


  @par How to use it ?

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Connect TIM1_CC1 to a "Low Pass" Filter (Rfilter =1.8Kohm, Cfilter= 0.47uF)
  - Connect the "Low Pass" filter output to an oscilloscope.

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
