/**
  ******************************************************************************
  * @file    TIM1_Sinewave_Generation\main.h
  * @author  MCD Application Team
  * @version V2.0.1
  * @date    18-November-2011
  * @brief   This file contains the defines for TIM1 Sinewave Generation example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private define ------------------------------------------------------------*/
#define FREQ_65_3595
/* #define FREQ_65_3083 */
/* #define FREQ_65_2316 */
/* #define FREQ_65_2061 */
/* #define FREQ_65_1890 */
/* #define FREQ_65_1042 */
/* #define FREQ_65_0407 */
/* #define FREQ_65_0280 */
/* #define FREQ_64_9351 */
/* #define FREQ_64_9013 */
/* #define FREQ_64_8340 */
/* #define FREQ_64_7501 */
/* #define FREQ_64_7249 */
/* #define FREQ_64_6412 */
/* #define FREQ_64_5995 */
/* #define FREQ_64_5661 */
/* #define FREQ_64_4745 */
/* #define FREQ_64_4122 */
/* #define FREQ_64_3501 */
/* #define FREQ_64_3004 */
/* #define FREQ_64_2013 */
/* #define FREQ_64_1026 */
/* #define FREQ_64_0615 */
/* #define FREQ_64_0041 */
/* #define FREQ_63_9304 */
/* #define FREQ_63_8366 */
/* #define FREQ_63_7755 */
/* #define FREQ_63_7105 */
/* #define FREQ_63_6618 */
/* #define FREQ_63_5728 */
/* #define FREQ_63_4921 */
/* #define FREQ_63_4196 */
/* #define FREQ_63_3955 */
/* #define FREQ_63_3112 */
/* #define FREQ_63_2111 */
/* #define FREQ_63_1313 */
/* #define FREQ_63_0517 */
/* #define FREQ_63_0040 */
/* #define FREQ_62_9327 */
/* #define FREQ_62_8457 */
/* #define FREQ_62_7943 */
/* #define FREQ_62_7510 */
/* #define FREQ_62_6566 */
/* #define FREQ_62_5626 */
/* #define FREQ_62_5000 */
/* #define FREQ_62_4376 */
/* #define FREQ_62_3830 */
/* #define FREQ_62_2859 */
/* #define FREQ_62_2355 */
/* #define FREQ_62_1388 */
/* #define FREQ_62_1118 */
/* #define FREQ_62_0347 */
/* #define FREQ_61_9348 */
/* #define FREQ_61_8429 */
/* #define FREQ_61_7856 */
/* #define FREQ_61_7284 */
/* #define FREQ_61_6371 */
/* #define FREQ_61_5385 */
/* #define FREQ_61_4439 */
/* #define FREQ_61_4213 */
/* #define FREQ_61_3422 */
/* #define FREQ_61_2595 */
/* #define FREQ_61_1621 */
/* #define FREQ_61_1172 */
/* #define FREQ_61_0501 */
/* #define FREQ_60_9719 */
/* #define FREQ_60_8828 */
/* #define FREQ_60_8162 */
/* #define FREQ_60_7607 */
/* #define FREQ_60_6870 */
/* #define FREQ_60_6061 */
/* #define FREQ_60_5180 */
/* #define FREQ_60_4741 */
/* #define FREQ_60_4047 */
/* #define FREQ_60_3318 */
/* #define FREQ_60_2773 */
/* #define FREQ_60_2228 */
/* #define FREQ_60_1902 */
/* #define FREQ_60_1251 */
/* #define FREQ_60_0601 */
/* #define FREQ_60_0240 */
/* #define FREQ_59_9305 */
/* #define FREQ_59_9089 */
/* #define FREQ_59_8480 */
/* #define FREQ_59_7907 */
/* #define FREQ_59_7729 */
/* #define FREQ_59_6516 */
/* #define FREQ_59_5735 */
/* #define FREQ_59_5238 */
/* #define FREQ_59_4177 */
/* #define FREQ_59_3542 */
/* #define FREQ_59_2592 */
/* #define FREQ_59_1856 */
/* #define FREQ_59_0902 */
/* #define FREQ_58_9971 */
/* #define FREQ_58_9345 */
/* #define FREQ_58_8512 */
/* #define FREQ_58_7889 */
/* #define FREQ_58_7372 */
/* #define FREQ_58_6441 */
/* #define FREQ_58_6029 */
/* #define FREQ_58_5103 */
/* #define FREQ_58_4795 */
/* #define FREQ_58_4112 */
/* #define FREQ_58_3567 */
/* #define FREQ_58_2751 */
/* #define FREQ_58_2242 */
/* #define FREQ_58_1395 */
/* #define FREQ_58_0720 */
/* #define FREQ_58_0114 */
/* #define FREQ_57_9609 */
/* #define FREQ_57_8704 */
/* #define FREQ_57_7901 */
/* #define FREQ_57_7501 */
/* #define FREQ_57_6901 */
/* #define FREQ_57_6037 */
/* #define FREQ_57_5109 */
/* #define FREQ_57_4911 */
/* #define FREQ_57_4218 */
/* #define FREQ_57_3394 */
/* #define FREQ_57_2344 */
/* #define FREQ_57_1559 */
/* #define FREQ_57_0776 */
/* #define FREQ_56_9801 */
/* #define FREQ_56_8925 */
/* #define FREQ_56_8182 */
/* #define FREQ_56_7279 */
/* #define FREQ_56_6316 */
/* #define FREQ_56_5611 */
/* #define FREQ_56_4780 */
/* #define FREQ_56_4207 */
/* #define FREQ_56_3730 */
/* #define FREQ_56_3063 */
/* #define FREQ_56_2303 */
/* #define FREQ_56_1545 */
/* #define FREQ_56_1167 */
/* #define FREQ_56_0538 */
/* #define FREQ_55_9848 */
/* #define FREQ_55_8909 */
/* #define FREQ_55_8628 */
/* #define FREQ_55_8036 */
/* #define FREQ_55_7414 */
/* #define FREQ_55_7041 */
/* #define FREQ_55_6297 */
/* #define FREQ_55_5556 */
/* #define FREQ_55_5001 */
/* #define FREQ_55_4324 */
/* #define FREQ_55_3710 */
/* #define FREQ_55_3097 */
/* #define FREQ_55_2608 */
/* #define FREQ_55_1633 */
/* #define FREQ_55_1146 */
/* #define FREQ_55_0237 */
/* #define FREQ_54_9451 */
/* #define FREQ_54_8968 */
/* #define FREQ_54_7975 */
/* #define FREQ_58_7345 */
/* #define FREQ_54_6717 */
/* #define FREQ_54_6329 */
/* #define FREQ_54_5554 */
/* #define FREQ_54_4662 */
/* #define FREQ_54_4010 */
/* #define FREQ_54_3241 */
/* #define FREQ_54_2535 */
/* #define FREQ_54_2005 */
/* #define FREQ_54_0950 */
/* #define FREQ_54_0774 */
/* #define FREQ_53_9811 */
/* #define FREQ_53_9374 */
/* #define FREQ_53_8677 */
/* #define FREQ_53_8097 */
/* #define FREQ_53_7634 */
/* #define FREQ_53_6769 */
/* #define FREQ_53_6423 */
/* #define FREQ_53_5447 */
/* #define FREQ_53_4445 */
/* #define FREQ_53_3504 */
/* #define FREQ_53_2822 */
/* #define FREQ_53_1971 */
/* #define FREQ_53_1011 */
/* #define FREQ_53_0223 */
/* #define FREQ_52_9773 */
/* #define FREQ_52_9101 */
/* #define FREQ_52_8541 */
/* #define FREQ_52_7649 */
/* #define FREQ_52_6759 */
/* #define FREQ_52_6094 */
/* #define FREQ_52_5100 */
/* #define FREQ_52_4439 */
/* #define FREQ_52_3670 */
/* #define FREQ_52_3122 */
/* #define FREQ_52_2575 */
/* #define FREQ_52_2139 */
/* #define FREQ_52_1268 */
/* #define FREQ_52_0915 */
/* #define FREQ_52_0075 */
/* #define FREQ_51_9481 */
/* #define FREQ_51_8888 */
/* #define FREQ_51_8242 */
/* #define FREQ_51_7411 */
/* #define FREQ_51_6529 */
/* #define FREQ_51_5677 */
/* #define FREQ_51_5145 */
/* #define FREQ_51_4192 */
/* #define FREQ_51_3321 */
/* #define FREQ_51_2715 */
/* #define FREQ_51_1876 */
/* #define FREQ_51_1300 */
/* #define FREQ_51_0308 */
/* #define FREQ_50_9580 */
/* #define FREQ_50_8751 */
/* #define FREQ_50_7924 */
/* #define FREQ_50_7305 */
/* #define FREQ_50_6586 */
/* #define FREQ_50_5663 */
/* #define FREQ_50_5051 */
/* #define FREQ_50_4439 */
/* #define FREQ_50_4032 */
/* #define FREQ_50_3221 */
/* #define FREQ_50_2311 */
/* #define FREQ_50_1781 */
/* #define FREQ_50_0801 */
/* #define FREQ_50_0200 */
/* #define FREQ_49_9500 */
/* #define FREQ_49_8604 */
/* #define FREQ_49_8107 */
/* #define FREQ_49_7240 */
/* #define FREQ_49_6327 */
/* #define FREQ_49_6032 */
/* #define FREQ_49_5148 */
/* #define FREQ_49_5000 */
/* #define FREQ_49_4071 */
/* #define FREQ_49_3681 */
/* #define FREQ_49_2732 */
/* #define FREQ_49_1836 */
/* #define FREQ_49_1352 */
/* #define FREQ_49_0581 */
/* #define FREQ_48_9620 */
/* #define FREQ_48_9045 */
/* #define FREQ_48_8400 */
/* #define FREQ_48_7424 */
/* #define FREQ_48_6760 */
/* #define FREQ_48_6239 */
/* #define FREQ_48_5248 */
/* #define FREQ_48_4496 */
/* #define FREQ_48_4097 */
/* #define FREQ_48_3302 */
/* #define FREQ_48_3092 */
/* #define FREQ_48_2253 */
/* #define FREQ_48_1974 */
/* #define FREQ_48_1000 */
/* #define FREQ_48_0031 */
/* #define FREQ_47_9869 */
/* #define FREQ_47_9478 */
/* #define FREQ_47_8721 */
/* #define FREQ_47_7829 */
/* #define FREQ_47_7464 */
/* #define FREQ_47_6463 */
/* #define FREQ_47_5647 */
/* #define FREQ_47_4654 */
/* #define FREQ_47_4226 */
/* #define FREQ_47_3485 */
/* #define FREQ_47_2590 */
/* #define FREQ_47_2010 */
/* #define FREQ_47_1343 */
/* #define FREQ_47_0544 */
/* #define FREQ_46_9814 */
/* #define FREQ_46_9219 */
/* #define FREQ_46_8516 */
/* #define FREQ_46_7639 */
/* #define FREQ_46_7115 */
/* #define FREQ_46_6505 */
/* #define FREQ_46_5636 */
/* #define FREQ_46_5030 */
/* #define FREQ_46_4511 */
/* #define FREQ_46_3671 */
/* #define FREQ_46_2963 */
/* #define FREQ_46_2535 */
/* #define FREQ_46_1723 */
/* #define FREQ_46_0914 */
/* #define FREQ_46_0575 */
/* #define FREQ_45_9791 */
/* #define FREQ_45_8884 */
/* #define FREQ_45_8631 */
/* #define FREQ_45_7875 */
/* #define FREQ_45_7059 */
/* #define FREQ_45_6704 */
/* #define FREQ_45_5976 */
/* #define FREQ_45_5000 */
/* #define FREQ_45_4091 */
/* #define FREQ_45_3515 */
/* #define FREQ_45_2960 */
/* #define FREQ_45_2223 */
/* #define FREQ_45_1671 */
/* #define FREQ_45_0938 */
/* #define FREQ_45_0369 */
/* #define FREQ_44_9843 */
/* #define FREQ_44_8934 */


#ifdef FREQ_65_3595
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_65_3083
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE7
       #define       INIT_TIM1_CCR1      0x73
       #define       AVERAGE_AMP         0x73
#endif

#ifdef FREQ_65_2316
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_65_2061
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_65_1890
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_65_1042
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xEF
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_65_0407
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_65_0280
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE8
       #define       INIT_TIM1_CCR1      0x74
       #define       AVERAGE_AMP         0x74
#endif

#ifdef FREQ_64_9351
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_64_9013
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_64_8340
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xF0
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_64_7501
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE9
       #define       INIT_TIM1_CCR1      0x74
       #define       AVERAGE_AMP         0x74
#endif

#ifdef FREQ_64_7249
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_64_6412
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_64_5995
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_64_5661
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xF1
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_64_4745
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEA
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_64_4122
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_64_3501
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_64_3004
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_64_2013
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_64_1026
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_64_0615
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_64_0041
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_63_8366
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF0
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_63_7755
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_63_7105
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_63_6618 
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_63_5728
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF1
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_63_4921
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_63_4196
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_63_3955 
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEE
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_63_3112
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF2
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_63_2111
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_63_1313
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_63_0517
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF3
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_63_0040
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xF1
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_62_9327
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_62_8457
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_62_7943
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF4
       #define       INIT_TIM1_CCR1      0x7A
       #define       AVERAGE_AMP         0x7A
#endif

#ifdef FREQ_62_7510
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xF2
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_62_6566
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE3
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_62_5626
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_62_5000
       #define       nbr_of_samples_half 16
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xF9
       #define       INIT_TIM1_CCR1      0x7C
       #define       AVERAGE_AMP         0x7C
#endif

#ifdef FREQ_62_4376 
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_62_3830
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE4
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_62_2859
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF6
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_62_2355
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_62_1388 
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD0
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_62_1118
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE5
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_62_0347
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF7
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_61_9348
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_61_8429
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD1
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_61_7856
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF8
       #define       INIT_TIM1_CCR1      0x7C
       #define       AVERAGE_AMP         0x7C
#endif

#ifdef FREQ_61_7284
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_61_6371
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_61_5385
       #define       nbr_of_samples_half 26
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF9
       #define       INIT_TIM1_CCR1      0x7C
       #define       AVERAGE_AMP         0x7C
#endif

#ifdef FREQ_61_4439
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_61_4213
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xC8
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_61_3422
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD0
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_61_2595
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_61_1621
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_61_1172
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xC9
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_61_0501
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD1
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_60_9719
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_60_8828
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_60_8162
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCA
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_60_7607
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_60_6870
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_60_6061
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_60_5180
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_60_4741
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_60_4047
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_60_3318
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_60_2773
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_60_2228
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_60_1902
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_60_1251
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_60_0601
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_60_0240
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_59_9305
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_59_9089
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_59_8480
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_59_7907
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_59_7729
       #define       nbr_of_samples_half 28
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xEE
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_59_6516
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xFD
       #define       INIT_TIM1_CCR1      0x7E
       #define       AVERAGE_AMP         0x7E
#endif

#ifdef FREQ_59_5735
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_59_5238
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_59_4177
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xFE
       #define       INIT_TIM1_CCR1      0x7F
       #define       AVERAGE_AMP         0x7F
#endif

#ifdef FREQ_59_3542
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_59_2592
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_59_1856
       #define       nbr_of_samples_half 22
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xFF
       #define       INIT_TIM1_CCR1      0x7F
       #define       AVERAGE_AMP         0x7F
#endif

#ifdef FREQ_59_0902
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD0
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_58_9971
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_58_9345
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xC9
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_58_8512
       #define       nbr_of_samples_half 16
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_58_7889
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD1
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_58_7372
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_58_6441
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCA
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_58_6029
       #define       nbr_of_samples_half 16
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_58_5103
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_58_4795
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE3
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_58_4112
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_58_3567
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_58_2751
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_58_2242
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xE4
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_58_1395
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_58_0720
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_58_0114
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_57_9609
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_57_8704
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_57_7901
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_57_7501
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_57_6901
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_57_6037
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_57_5109 
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_57_4911
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_57_4218
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_57_3394
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_57_2344
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_57_1559
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_57_0776
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_56_9801
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_56_8925
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_56_8182
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_56_7279
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_56_6316
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_56_5611
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_56_4780
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_56_4207
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_56_3730
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_56_3063
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_56_2303
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE3
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_56_1545
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_56_1167
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_56_0538
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_55_9848
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE4
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_55_8909
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_55_8628
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_55_8036
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_55_7414
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE5
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_55_7041
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_55_6297
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_55_5556
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_55_5001
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE6
       #define       INIT_TIM1_CCR1      0x73
       #define       AVERAGE_AMP         0x73
#endif

#ifdef FREQ_55_4324
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_55_3710
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_55_3097
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_55_2608
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE7
       #define       INIT_TIM1_CCR1      0x73
       #define       AVERAGE_AMP         0x73
#endif

#ifdef FREQ_55_1633
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_55_1146
       #define       nbr_of_samples_half 28
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_55_0237
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xE8
       #define       INIT_TIM1_CCR1      0x74
       #define       AVERAGE_AMP         0x74
#endif

#ifdef FREQ_54_9451
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xC7
       #define       INIT_TIM1_CCR1      0x63
       #define       AVERAGE_AMP         0x63
#endif

#ifdef FREQ_54_8968
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_54_7975
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_58_7345
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCA
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_54_6717
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xC8
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_54_6329
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_54_5554
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEA
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_54_4662
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_54_4010
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xC9
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_54_3241
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_54_2535
       #define       nbr_of_samples_half 24
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xFF
       #define       INIT_TIM1_CCR1      0x7F
       #define       AVERAGE_AMP         0x7F
#endif

#ifdef FREQ_54_2005
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_54_0950
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_54_0774
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xC8
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_53_9811
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF6
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_53_9374
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_53_8677
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_53_8097
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xC9
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_53_7634
       #define       nbr_of_samples_half 30
       #define       COUNTER             5
       #define       INIT_TIM1_ARR       0xF7
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_53_6769
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_53_6423
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xEE
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_53_5447
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCA
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_53_4445
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF2
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_53_3504
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_53_2822
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_53_1971
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xF0
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_53_1011
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_53_0223
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_52_9773
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xF1
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_52_9101
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_52_8541
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_52_7649
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_52_6759
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_52_6094
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_52_5100
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_52_4439
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_52_3670
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_52_3122
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_52_2575
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_52_2139
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE3
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_52_1268
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_52_0915
       #define       nbr_of_samples_half 18
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_52_0075
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD0
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_51_9481
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF9
       #define       INIT_TIM1_CCR1      0x7C
       #define       AVERAGE_AMP         0x7C
#endif

#ifdef FREQ_51_8888
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_51_8242
       #define       nbr_of_samples_half 24
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xC8
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_51_7411
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xFA
       #define       INIT_TIM1_CCR1      0x7D
       #define       AVERAGE_AMP         0x7D
#endif

#ifdef FREQ_51_6529
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_51_5677
       #define       nbr_of_samples_half 24
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xC9
       #define       INIT_TIM1_CCR1      0x64
       #define       AVERAGE_AMP         0x64
#endif

#ifdef FREQ_51_5145
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_51_4192
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_51_3321
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xFC
       #define       INIT_TIM1_CCR1      0x7E
       #define       AVERAGE_AMP         0x7E
#endif

#ifdef FREQ_51_2715
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_51_1876
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_51_1300
       #define       nbr_of_samples_half 22
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xFD
       #define       INIT_TIM1_CCR1      0x7E
       #define       AVERAGE_AMP         0x7E
#endif

#ifdef FREQ_51_0308
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_50_9580
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_50_8751
       #define       nbr_of_samples_half 26
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xFB
       #define       INIT_TIM1_CCR1      0x7D
       #define       AVERAGE_AMP         0x7D
#endif

#ifdef FREQ_50_7924
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_50_7305
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_50_6586
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEA
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_50_5663
       #define       nbr_of_samples_half 24
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xCD
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_50_5051
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_50_4439
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_50_4032
       #define       nbr_of_samples_half 16
       #define       COUNTER             10
       #define       INIT_TIM1_ARR       0xF7
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_50_3221
       #define       nbr_of_samples_half 24
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xCE
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_50_2311
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_50_1781
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_50_0801
       #define       nbr_of_samples_half 24
       #define       COUNTER             8
       #define       INIT_TIM1_ARR       0xCF
       #define       INIT_TIM1_CCR1      0x67
       #define       AVERAGE_AMP         0x67
#endif

#ifdef FREQ_50_0200
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_49_9500
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_49_8604
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_49_8107
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEE
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_49_7240
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_49_6327
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_49_6032
       #define       nbr_of_samples_half 24
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEF
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_49_5148
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCB
       #define       INIT_TIM1_CCR1      0x65
       #define       AVERAGE_AMP         0x65
#endif

#ifdef FREQ_49_5000
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_49_4071
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_49_3681
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_49_2732
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xCC
       #define       INIT_TIM1_CCR1      0x66
       #define       AVERAGE_AMP         0x66
#endif

#ifdef FREQ_49_1836
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_49_1352
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_49_0581
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_48_9620
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_48_9045
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_48_8400
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_48_7424
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_48_6760
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_48_6239
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_48_5248
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_48_4496
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_48_4097
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_48_3302
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD0
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_48_3092
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_48_2253
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_48_1974
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE3
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_48_1000
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD1
       #define       INIT_TIM1_CCR1      0x68
       #define       AVERAGE_AMP         0x68
#endif

#ifdef FREQ_48_0031
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD8
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_47_9869
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xE4
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_47_9478
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_47_8721
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD2
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_47_7829
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xD9
       #define       INIT_TIM1_CCR1      0x6C
       #define       AVERAGE_AMP         0x6C
#endif

#ifdef FREQ_47_7464
       #define       nbr_of_samples_half 16
       #define       COUNTER             11
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_47_6463
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD3
       #define       INIT_TIM1_CCR1      0x69
       #define       AVERAGE_AMP         0x69
#endif

#ifdef FREQ_47_5647
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDA
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_47_4654
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE4
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_47_4226
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD4
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_47_3485
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDB
       #define       INIT_TIM1_CCR1      0x6D
       #define       AVERAGE_AMP         0x6D
#endif

#ifdef FREQ_47_2590
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE5
       #define       INIT_TIM1_CCR1      0x72
       #define       AVERAGE_AMP         0x72
#endif

#ifdef FREQ_47_2010
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD5
       #define       INIT_TIM1_CCR1      0x6A
       #define       AVERAGE_AMP         0x6A
#endif

#ifdef FREQ_47_1343
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_47_0544
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE6
       #define       INIT_TIM1_CCR1      0x73
       #define       AVERAGE_AMP         0x73
#endif

#ifdef FREQ_46_9814
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD6
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_46_9219
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_46_8516
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE7
       #define       INIT_TIM1_CCR1      0x73
       #define       AVERAGE_AMP         0x73
#endif

#ifdef FREQ_46_7639
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xD7
       #define       INIT_TIM1_CCR1      0x6B
       #define       AVERAGE_AMP         0x6B
#endif

#ifdef FREQ_46_7115
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_46_6505
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE8
       #define       INIT_TIM1_CCR1      0x74
       #define       AVERAGE_AMP         0x74
#endif

#ifdef FREQ_46_5636
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_46_5030
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_46_4511
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xE9
       #define       INIT_TIM1_CCR1      0x74
       #define       AVERAGE_AMP         0x74
#endif

#ifdef FREQ_46_3671
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_46_2963
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_46_2535
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xEA
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_46_1723
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_46_0914
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xE1
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif

#ifdef FREQ_46_0575
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xEB
       #define       INIT_TIM1_CCR1      0x75
       #define       AVERAGE_AMP         0x75
#endif

#ifdef FREQ_45_9791
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEE
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_45_8884
       #define       nbr_of_samples_half 16
       #define       COUNTER             12
       #define       INIT_TIM1_ARR       0xE2
       #define       INIT_TIM1_CCR1      0x71
       #define       AVERAGE_AMP         0x71
#endif

#ifdef FREQ_45_8631
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xEC
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_45_7875
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xEF
       #define       INIT_TIM1_CCR1      0x77
       #define       AVERAGE_AMP         0x77
#endif

#ifdef FREQ_45_7059
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDC
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_45_6704
       #define       nbr_of_samples_half 46
       #define       COUNTER             4
       #define       INIT_TIM1_ARR       0xED
       #define       INIT_TIM1_CCR1      0x76
       #define       AVERAGE_AMP         0x76
#endif

#ifdef FREQ_45_5976
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF0
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_45_5000
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDD
       #define       INIT_TIM1_CCR1      0x6E
       #define       AVERAGE_AMP         0x6E
#endif

#ifdef FREQ_45_4091
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF1
       #define       INIT_TIM1_CCR1      0x78
       #define       AVERAGE_AMP         0x78
#endif

#ifdef FREQ_45_3515
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xF4
       #define       INIT_TIM1_CCR1      0x7A
       #define       AVERAGE_AMP         0x7A
#endif

#ifdef FREQ_45_2960
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDE
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_45_2223
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF2
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_45_1671
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xF5
       #define       INIT_TIM1_CCR1      0x7A
       #define       AVERAGE_AMP         0x7A
#endif

#ifdef FREQ_45_0938
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xDF
       #define       INIT_TIM1_CCR1      0x6F
       #define       AVERAGE_AMP         0x6F
#endif

#ifdef FREQ_45_0369
       #define       nbr_of_samples_half 26
       #define       COUNTER             7
       #define       INIT_TIM1_ARR       0xF3
       #define       INIT_TIM1_CCR1      0x79
       #define       AVERAGE_AMP         0x79
#endif

#ifdef FREQ_44_9843
       #define       nbr_of_samples_half 30
       #define       COUNTER             6
       #define       INIT_TIM1_ARR       0xF6
       #define       INIT_TIM1_CCR1      0x7B
       #define       AVERAGE_AMP         0x7B
#endif

#ifdef FREQ_44_8934
       #define       nbr_of_samples_half 22
       #define       COUNTER             9
       #define       INIT_TIM1_ARR       0xE0
       #define       INIT_TIM1_CCR1      0x70
       #define       AVERAGE_AMP         0x70
#endif


/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint8_t Value[nbr_of_samples_half];
  uint8_t Index;
  bool Up;
}MySin_Typedef;



/*** (c) 2008   STMicroelectronics ****************** END OF FILE ************/
