/** @page Networking  Networking with CAN to control Leds
  @verbatim
  ******************** (C)COPYRIGHT 2014 STMicroelectronics *******************
  * @file    CAN/CAN_Networking/readme.txt
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains the main function for CAN networking example.
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

  This example provides a short description of how to use the CAN peripheral:
  CAN peripheral is configured to send and receive CAN frames. 
  The sent frames  are used to control Leds.
  An LCD is used to give information about : 
   - CAN sender (sender of the frame)
   - CAN receiver (receiver of the frame)
   - Controlled Led (set ON)

  The CAN serial communication link is a bus to which a number of units may be
  connected. This number has no theoretical limit. Practically the total number
  of units will be limited by delay times and/or electrical loads on the bus line.

  @note This example is tested with a bus of 3 units. The same program example 
  is loaded in all units to send and receive frames.

  CAN is configured as follow:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = Internal (HSI)
    - ID Filter = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte

  @par Directory contents

  - CAN\CAN_Networking\main.c             Main file containing the "main" function
  - CAN\CAN_Networking\main.h             Main file declaration
  - CAN\CAN_Networking\stm8s_conf.h       Library Configuration file
  - CAN\CAN_Networking\stm8s_it.c         Interrupt routines source 
  - CAN\CAN_Networking\stm8s_it.h         Interrupt routines declaration
  

  @par Hardware environment

  - This example runs only on STM8S208 and STM8A52Ax devices.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation 
    board and can be easily tailored to any other development board.

  - STM8/128-EVAL Set-up
     - Key push button
     - LD1..LD4
     - Mono LCD

 
  - If another board is used, consider the folloing hardware connections :
      - PH3 -> LED1 -> resistor -> ground
      - PH2 -> LED2 -> resistor -> ground
      - PH1 -> LED3 -> resistor -> ground
      - PH0 -> LED4 -> resistor -> ground
      - PC0 -> Key Button -> ground + pull-up on 5V
      - PG1 (CAN Rx) and PG0 (CAN Tx) connected to a DB9 connector using a CAN Bus
        transceiver (eg. L9616).

  - Connect a female/female CAN cable between at least 2 EVAL CAN connectors
  (CN8 on STM8/128-EVAL board) 
    - Connector 1 DB9_PIN2 to Connector 2 DB9_PIN2  (CAN_L)
    - Connector 1 DB9_PIN5 to Connector 2 DB9_PIN5  ( GND )
    - Connector 1 DB9_PIN7 to Connector 2 DB9_PIN7  (CAN_H)


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8s_eval_lcd.c  (under Utilities\STM8S_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  - In the first time the LCD shows the message "STM8S CAN  Press Key".
  - (1) By Pressing on Key Button : CAN send a Frame to command LED1 (LD1 ON)
    on the N eval-boards connected to the bus, all other Leds are OFF. 
    The sender LCD shows "STM8S CAN  Sender  LD1 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD1 ON".
  - Press on Key Button again to send CAN Frame to command LD2 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD2 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD2 ON".
  - Press on Key Button again to send CAN Frame to command LD3 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD3 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD3 ON".
  - Press on Key Button again to send CAN Frame to command LD4 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD4 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD4 ON".
  - Press on Key Button again to go to step (1)
  @note Any unit in the CAN bus may play the role of sender (by pressing on the
        key) or receiver.

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
