/**
  @page exampletemplate Project Example Template
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This sub-directory contains all the user-modifiable files 
  *          needed to create a new project linked with the STM8S/A  
  *          Standard Peripheral Library.
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

@par Project Example Template Description 

This example is used as a template that can be used as reference to build any new 
firmware development for all STM8S and STM8A product devices using the STM8S/A 
Standard Peripherals Library.


@par Directory contents 

This folder contains standard template projects for EWSTM8, STVD with Cosmic, 
STVD with raisonance and RIDE toolchains that compile the needed STM8S/A Standard
Peripheral's drivers plus all the user-modifiable files that are necessary to 
create a new project. 

  - Project/Template/stm8s_conf.h    Library Configuration file

  - Project/Template/stm8s_it.c:     Source file containing the interrupt handlers (the 
                                     function bodies are empty in this template).

  - Project/Template/stm8s_it.h      Header file including all interrupt handlers prototypes.

  - Project/Template/main.c          Main program
          
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
