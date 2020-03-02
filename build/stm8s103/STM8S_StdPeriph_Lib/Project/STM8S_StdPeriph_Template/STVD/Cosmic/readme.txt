/** @page project Template project for ST Visual Develop (STVD) toolchain with Cosmic compiler
    
  @verbatim
  ******************* (C) COPYRIGHT 2011 STMicroelectronics ********************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   This sub-directory contains all the user-modifiable files needed
  *          to create a new project linked with the STM8S/A Standard Peripheral 
  *          Library and working with STVD and Cosmic software toolchain.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  
  @par Project description

  This folder contains a standard STVD template workspace that includes 11 
  projects related to the FW Lib supported products.
  Each project includes all the user-modifiable files that are necessary 
  to create a new project.
  These project templates can be used by mean of minor updates in the library files
  to run the FWLib examples, or custom user applications. 

  
  - project\\template\\STVD\\Cosmic
     - project.stw              Workspace file
     - stm8s103.stp             Project file for STM8S103 Low-density devices. 
     - stm8s003.stp             Project file for STM8S003 Low-density devices.
     - stm8s105.stp             Project file for STM8S105 Medium-density devices.
     - stm8s005.stp             Project file for STM8S005 Medium-density devices.
     - stm8s207.stp             Project file for STM8S207 High-density devices.
     - stm8s007.stp             Project file for STM8S007 High-density devices.
     - stm8s208.stp             Project file for STM8S208 High-density devices.
     - stm8s903.stp             Project file for STM8S903 Low-density devices.
     - stm8af52ax.stp           Project file for STM8AF52Ax High-density devices.
     - stm8af62ax.stp           Project file for STM8AF62Ax High-density devices.
     - stm8af626x.stp           Project file for STM8A Medium-density devices.


  @par How to use it ?

  - Open the STVD workspace
  - Set active the project related to the used product: Project-> Set Active Project, 
  and choose the desired project 
  - Select your debug instrument: Debug instrument-> Target Settings, select the 
  target you want to use for debug session (Swim Stice or Swim Rlink)
  - Rebuild all files: Build-> Rebuild all. 
  - Load project image: Debug->Start Debugging
  - Run program: Debug->Run (Ctrl+F5)
  
  @b Tip: If it is your first time using STVD, you have to confirm the default 
  toolset and path information that will be used when building your application, 
  to do so:
    - Select Tools-> Options
    - In the Options window click on the Toolset tab
    - Select your toolset from the Toolset list box.
    If the path is incorrect you can type the correct path in the Root Path 
    field, or use the browse button to locate it.
    - In the subpath fields, type the correct subpath if necessary 
  
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
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
