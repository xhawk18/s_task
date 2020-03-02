/**
  @page ewstm8 EWSTM8 Project Template for STM8S/A product devices
  
  @verbatim
  ******************* (C) COPYRIGHT 2011 STMicroelectronics ********************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   This sub-directory contains all the user-modifiable files needed
  *          to create a new project linked with the STM8S/A Standard Peripheral 
  *          Library and working with IAR Embedded Workbench for STM8 (EWSTM8)
  *          software toolchain.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
 
 @par Directory contents

 - project .ewd/.eww/.ewp: A pre-configured project file with the provided library 
                           structure that produces an executable image with IAR 
                           Embedded Workbench.
                
                             
 @par How to use it ?

 - Open the Project.eww workspace.
 - In the workspace toolbar select the project configuration:
     - stm8af622x: to configure the project for STM8AF622x Low-density devices.
     - stm8s103: to configure the project for STM8S103 Low-density devices. 
     - stm8s903: to configure the project for STM8S903 Low-density devices.
     - stm8s105: to configure the project for STM8S Medium-density devices.
     - stm8s207: to configure the project for STM8S207 High-density devices.
     - stm8s208: to configure the project for STM8S208 High-density devices.
     - stm8af52ax: to configure the project for STM8AF52Ax High-density devices.
     - stm8af62ax: to configure the project for STM8AF62Ax High-density devices.
     - stm8af626x: to configure the project for STM8A Medium-density devices.
     @note The needed define symbols for these configurations are already declared in the
           preprocessor section.

 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Debug
 - Run program: Debug->Go(F5)

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
  - Low-Density STM8A devices are the STM8AF622x microcontrollers where the Flash
    density is 8 Kbytes.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
