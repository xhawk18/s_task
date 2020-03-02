/**
  @page I2C_DataExchange I2C Communication Boards Data Exchange example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards/I2C_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Description of the I2C Communication Boards Data Exchange example.
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

This is a typical example of how to use the I2C software library to ensure the
steps of an I2C communication between slave Receiver/transmitter and master 
transmitter/receiver using interrupts.

- Hardware Description

To use this example, you need to load it on two STM8 boards (let's call them 
Board A and Board B) then connect these two boards through I2C lines and GND.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |I2C Master|____|_______SDA_______|____|I2C Slave |    |         |
|        |    |  Device  |____|_______SCL_______|____|  Device  |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2     RESET   |                 |  O LD2     RESET   |         |
|        |               _    |                 |               _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

This example requires two boards(Master and Slave)

  - Phase 1:
    In master board I2C peripheral is configured in Master Transmitter with Interrupt,
    where in Slave board I2C peripheral is configured in Slave Receiver with Interrupt.
    The master will send Data to Slave according to the defined buffer size.
   
  - Phase 2: 
    In master board I2C peripheral is configured in Master Receiver in polling mode,
    where in Slave board I2C peripheral is configured in Slave Transmitter with 
    Interrupt. 
    Once the master has initiated the communication (Start condition), the slave
    will send back the data to the master according to the defined buffer size.
   
At the end of the data transfer, a data consistency check will be performed 
in master sides. In case of mismatch between sent and received data, LED4 will toggle.
Otherwise LED1 will toggle. 

Communication error are handled in slave sides, their occurrence is signaled by 
puting LED2 On.
  

The File "main.h" allows different possibilities of configuration of this example:
  - Addressing mode : 7-bit or 10-bit
  - I2C speed: Fast mode or Standard mode


@par Directory contents 

  - I2C/I2C_TwoBoards/I2C_DataExchange/X/stm8s_conf.h    Library Configuration file
  - I2C/I2C_TwoBoards/I2C_DataExchange/X/stm8s_it.c      Interrupt handlers
  - I2C/I2C_TwoBoards/I2C_DataExchange/X/stm8s_it.h      Interrupt handlers header file
  - I2C/I2C_TwoBoards/I2C_DataExchange/X/main.c          Main program
  - I2C/I2C_TwoBoards/I2C_DataExchange/X/main.h          Main Header file
  
  - X stands for Master or Slave
  
         
@par Hardware and Software environment

  - This example runs on STM8S and STM8A High density, Medium density and Low 
    density devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
    - LED1, LED2 and LED4
    - Connect I2C Master SCL pin (PE.01) to I2C Slave SCL (PE.01)
    - Connect I2C Master SDA pin (PE.02) to I2C Slave SDA (PE.02)

   @note  
    - Pull-up resistors should be connected to I2C SCL and SDA Pins in case of 
      other boards use.
      Pull-up  resistors are already implemented on the STM8/128-EVAL evaluation 
      boards.  
        
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
