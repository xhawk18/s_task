/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2011 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"


uint32_t g_au32DataArrary[4] = {0x00000000, 0xFFFFFFFF, 0x5A5A5A5A, 0xA5A5A5A5};
void SRAM_CompareFunction(uint32_t u32Param0, uint32_t u32Param1, uint32_t u32Param2, uint32_t u32Param3)
{
	uint8_t 	*pu8DevPtr;
	uint16_t 	*pu16DevPtr;
	uint32_t 	*pu32DevPtr;
	uint32_t 	u32ReadData, u32CmpData;	
	uint32_t 	u32Idx;	
	uint32_t 	u32EBIsize = u32Param1;
	uint32_t 	u32Offset = u32Param2;
	uint32_t 	u32WriteMode = u32Param3;
	uint8_t 	u8D0, u8D1, u8D2, u8D3;
	uint16_t 	u16D0, u16D1;
	
	if (u32Param0 == 4)		// 0x0, 0x1, 0x2, 0x3, 0x4 ... Access with offset ......
	{
		// Unfixed data pattern
		// One Byte Read
		u32Idx = 0;
		u32ReadData = 0;
		pu8DevPtr = (uint8_t *)EBI_START_BASE_ADDR;
		while (u32Idx < u32EBIsize)					
		{
			if (u32WriteMode == 0)	// From Byte Write Mode
			{
				u32CmpData = (uint8_t)(u32ReadData + u32Offset);
				if (*pu8DevPtr++ != (uint8_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", *(--pu8DevPtr));
					printf("SRAM want data= 0x%x\n", (uint8_t)(u32CmpData));	
					printf("One Byte Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx++;   
				u32ReadData++;
			}else
			if (u32WriteMode == 1)	// From Half Word Write Mode
			{
				u8D0 = (uint8_t)*pu8DevPtr++;
				u8D1 = (uint8_t)*pu8DevPtr++;
				u32CmpData = (uint16_t)(u8D0 + (u8D1<<8));
				if ((uint16_t)(u32ReadData+u32Offset) != (uint16_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", (uint16_t)u32CmpData);
					printf("SRAM want data= 0x%x\n", (uint16_t)((u32ReadData+u32Offset)));	
					printf("One Byte Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 2;   
				u32ReadData++;
			}else
			if (u32WriteMode == 2)	// From One Word Write Mode
			{
				u8D0 = (uint8_t)*pu8DevPtr++;
				u8D1 = (uint8_t)*pu8DevPtr++;
				u8D2 = (uint8_t)*pu8DevPtr++;
				u8D3 = (uint8_t)*pu8DevPtr++;
				u32CmpData = (uint32_t)(u8D0 + (u8D1<<8) + (u8D2<<16) + (u8D3<<24));
				if ((uint32_t)(u32ReadData+u32Offset) != (uint32_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", (uint32_t)u32CmpData);
					printf("SRAM want data= 0x%x\n", (uint32_t)((u32ReadData+u32Offset)));	
					printf("One Byte Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 4;   
				u32ReadData++;
			}
		}	
		if ((uint32_t)pu8DevPtr <  (EBI_START_BASE_ADDR+u32EBIsize))
		{
			printf("One Byte compare address error 0x%X !\n", (uint32_t)pu8DevPtr);
			while(1);
		}
		
		// Half Word Read
		u32Idx = 0;
		u32ReadData = 0;
		pu16DevPtr = (uint16_t *)EBI_START_BASE_ADDR;
		while (u32Idx < u32EBIsize)					
		{
			if (u32WriteMode == 0)	// From Byte Write Mode
			{
				u8D0 = (uint8_t)(u32ReadData + u32Offset);
				u8D1 = (uint8_t)(u32ReadData + u32Offset + 1);
				u32CmpData = (uint16_t)(u8D0 + (u8D1<<8));
				if (*pu16DevPtr++ != (uint16_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
					printf("SRAM read data= 0x%X\n", *(--pu16DevPtr));
					printf("SRAM want data= 0x%X\n", (uint16_t)(u32CmpData));	
					printf("Half Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 2;   
				u32ReadData = u32ReadData + 2;
			}else
			if (u32WriteMode == 1)	// From Half Word Write Mode
			{
				u32CmpData = (uint16_t)(u32ReadData + u32Offset);
				if (*pu16DevPtr++ != (uint16_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", *(--pu16DevPtr));
					printf("SRAM want data= 0x%x\n", (uint16_t)(u32CmpData));	
					printf("Half Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 2;   
				u32ReadData++;
			}else
			if (u32WriteMode == 2)	// From One Word Write Mode
			{
				u16D0 = (uint16_t)*pu16DevPtr++;
				u16D1 = (uint16_t)*pu16DevPtr++;
				u32CmpData = (uint32_t)(u16D0 + (u16D1<<16));
				if ((uint32_t)(u32ReadData+u32Offset) != (uint32_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", (uint32_t)u32CmpData);
					printf("SRAM want data= 0x%x\n", (uint32_t)((u32ReadData+u32Offset)));	
					printf("Half Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 4;   
				u32ReadData++;
			}
		}	
		if ((uint32_t)pu16DevPtr <  (EBI_START_BASE_ADDR+u32EBIsize))
		{
			printf("Half Word compare address error 0x%X !\n", (uint32_t)pu16DevPtr);
			while(1);
		}
		
		// One Word Read
		u32Idx = 0;
		u32ReadData = 0;
		pu32DevPtr = (uint32_t *)EBI_START_BASE_ADDR;
		while (u32Idx < u32EBIsize)					
		{
			if (u32WriteMode == 0)	// From Byte Write Mode
			{
				u8D0 = (uint8_t)(u32ReadData + u32Offset);
				u8D1 = (uint8_t)(u32ReadData + u32Offset + 1);
				u8D2 = (uint8_t)(u32ReadData + u32Offset + 2);
				u8D3 = (uint8_t)(u32ReadData + u32Offset + 3);
				u32CmpData = (uint32_t)(u8D0 + (u8D1<<8) + (u8D2<<16) + (u8D3<<24));
				if (*pu32DevPtr++ != (uint32_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
					printf("SRAM read data= 0x%X\n", *(--pu32DevPtr));
					printf("SRAM want data= 0x%X\n", (uint32_t)(u32CmpData));	
					printf("One Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 4;   
				u32ReadData = u32ReadData + 4;
			}else
			if (u32WriteMode == 1)	// From Half Word Write Mode
			{
				u16D0 = (uint16_t)(u32ReadData + u32Offset);
				u16D1 = (uint16_t)(u32ReadData + u32Offset + 1);
				u32CmpData = (uint32_t)(u16D0 + (u16D1<<16));
				if (*pu32DevPtr++ != (uint32_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
					printf("SRAM read data= 0x%X\n", *(--pu32DevPtr));
					printf("SRAM want data= 0x%X\n", (uint32_t)(u32CmpData));	
					printf("One Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 4;   
				u32ReadData = u32ReadData + 2;
			}else
			if (u32WriteMode == 2)	// From One Word Write Mode
			{
				u32CmpData = (uint32_t)(u32ReadData + u32Offset);
				if (*pu32DevPtr++ != (uint32_t)(u32CmpData))
				{
					printf("\nSRAM compare fail, Addr= 0x%x\n", u32Idx);
					printf("SRAM read data= 0x%x\n", *(--pu32DevPtr));
					printf("SRAM want data= 0x%x\n", (uint32_t)(u32CmpData));	
					printf("One Word Read compare failed ! \n\n");	
					while(1);	
				}	
				u32Idx = u32Idx + 4;   
				u32ReadData++;
			}
		}	
		if ((uint32_t)pu32DevPtr <  (EBI_START_BASE_ADDR+u32EBIsize))
		{
			printf("One Word compare address error 0x%X !\n", (uint32_t)pu32DevPtr);
			while(1);
		}
	}else
	{
		// Fixed data pattern
		// One Byte Read
		u32Idx = 0;
		pu8DevPtr = (uint8_t *)EBI_START_BASE_ADDR;
		u32ReadData = (uint8_t)g_au32DataArrary[u32Param0];
		while (u32Idx < u32EBIsize)					
		{
			if (*pu8DevPtr++ != (uint8_t)(u32ReadData))
			{
				printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
				printf("SRAM read data= 0x%X\n", *(--pu8DevPtr));
				printf("SRAM want data= 0x%X\n", (uint8_t)(u32ReadData));	
				printf("One Byte Read compare failed ! \n\n");	
				while(1);	
			}	
			u32Idx = u32Idx + 1;   
		}	
		
		// Half Word Read
		u32Idx = 0;
		pu16DevPtr = (uint16_t *)EBI_START_BASE_ADDR;
		u32ReadData = (uint16_t)g_au32DataArrary[u32Param0];
		while (u32Idx < u32EBIsize)					
		{
			if (*pu16DevPtr++ != (uint16_t)(u32ReadData))
			{
				printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
				printf("SRAM read data= 0x%X\n", *(--pu16DevPtr));
				printf("SRAM want data= 0x%X\n", (uint16_t)(u32ReadData));	
				printf("Half Word Read compare failed ! \n\n");	
				while(1);	
			}	
			u32Idx = u32Idx + 2;   
		}	
		
		// One Word Read
		u32Idx = 0;
		pu32DevPtr = (uint32_t *)EBI_START_BASE_ADDR;
		u32ReadData = (uint32_t)g_au32DataArrary[u32Param0];
		while (u32Idx < u32EBIsize)					
		{
			if (*pu32DevPtr++ != (uint32_t)(u32ReadData))
			{
				printf("\nSRAM compare fail, Addr= 0x%X\n", u32Idx);
				printf("SRAM read data= 0x%X\n", *(--pu32DevPtr));
				printf("SRAM want data= 0x%X\n", (uint32_t)(u32ReadData));	
				printf("One Word Read compare failed ! \n\n");	
				while(1);	
			}	
			u32Idx = u32Idx + 4;   
		}	
	}
}

/* Data Width : 16bits, Data Size : 128K  */
/* 1. One Byte Write                      */
/* 2. Half Word Write                     */
/* 3. Onw Word Write                      */
void SRAM_BS616LV4017(void)
{
	uint16_t *pu16DevPtr;
	uint32_t *pu32DevPtr;
	uint32_t u32WriteData;
	uint32_t u32Idx, u32DataIdx;	
	uint32_t u32EBIsize;

    u32EBIsize = EBI_MAX_SIZE;
    printf("   >> Data Width use 16bits and Data Size is %dKB << \n", (u32EBIsize/1024));
	
#if 1   
printf("\n Only M05xBN series support 16-bit data width with byte access. \n");
{
	/* One Byte Write */
	uint8_t *pu8DevPtr;
	printf("[[ One Byte Write test ]]\n");
	u32DataIdx = 0;
	while (u32DataIdx < 5)
	{
		if (u32DataIdx < 4)
		{
			// Fixed data pattern
			// Write to SRAM
			u32Idx = 0;
			pu8DevPtr = (uint8_t *)EBI_START_BASE_ADDR;
			u32WriteData = g_au32DataArrary[u32DataIdx];
			printf("	All 0x%X Access ... ", (uint8_t)u32WriteData);
			while (u32Idx < u32EBIsize)					
			{
				*pu8DevPtr++ = (uint8_t)(u32WriteData);
				u32Idx = u32Idx + 1;   
			}
			// Read from SRAM and compare data
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, 0, 0);	// param3:0, Byte Write
			printf("PASS\n");
		}else
		{
			// Unfixed data pattern
			// Write to SRAM
			u32Idx = 0;
			u32WriteData = 0;
			pu8DevPtr = (uint8_t *)EBI_START_BASE_ADDR;
			printf("	0x0, 0x1, 0x2, 0x3, 0x4 ... Access with offset(0x%X) ... ", (uint32_t)&u32Idx);
			while (u32Idx < u32EBIsize)					
			{
				*pu8DevPtr++ = (uint8_t)(u32WriteData + (uint32_t)&u32Idx);
				u32Idx = u32Idx + 1;   
				u32WriteData++;
			}
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, (uint32_t)&u32Idx, 0);	// param3:0, Byte Write
			printf("PASS\n\n");	
		}
		u32DataIdx++;
	}
}
#endif
	
	/* Half Word Write */
	printf("[[ Half Word Write test]]\n");
	u32DataIdx = 0;
	while (u32DataIdx < 5)
	{
		if (u32DataIdx < 4)
		{
			// Fixed data pattern
			// Write to SRAM
			u32Idx = 0;
			pu16DevPtr = (uint16_t *)EBI_START_BASE_ADDR;
			u32WriteData = g_au32DataArrary[u32DataIdx];
			printf("	All 0x%X Access ... ", (uint16_t)u32WriteData);
			while (u32Idx < u32EBIsize)					
			{
				*pu16DevPtr++ = (uint16_t)(u32WriteData);
				u32Idx = u32Idx + 2;   
			}
			// Read from SRAM and compare data
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, 0, 1);	// param3:1, Half Word Write
			printf("PASS\n");
		}else
		{
			// Unfixed data pattern
			// Write to SRAM
			u32Idx = 0;
			u32WriteData = 0;
			pu16DevPtr = (uint16_t *)EBI_START_BASE_ADDR;
			printf("	0x0, 0x1, 0x2, 0x3, 0x4 ... Access with offset(0x%X) ... ", (uint32_t)&u32Idx);
			while (u32Idx < u32EBIsize)					
			{
				*pu16DevPtr++ = (uint16_t)(u32WriteData + (uint32_t)&u32Idx);
				u32Idx = u32Idx + 2;   
				u32WriteData++;
			}
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, (uint32_t)&u32Idx, 1);	// param3:1, Half Word Write
			printf("PASS\n\n");	
		}
		u32DataIdx++;
	}
	
	/* One Word Write */
	printf("[[ One Word Write test ]]\n");
	u32DataIdx = 0;
	while (u32DataIdx < 5)
	{
		if (u32DataIdx < 4)
		{
			// Fixed data pattern
			// Write to SRAM
			u32Idx = 0;
			pu32DevPtr = (uint32_t *)EBI_START_BASE_ADDR;
			u32WriteData = g_au32DataArrary[u32DataIdx];
			printf("	All 0x%X Access ... ", (uint32_t)u32WriteData);
			while (u32Idx < u32EBIsize)					
			{
				*pu32DevPtr++ = (uint32_t)(u32WriteData);
				u32Idx = u32Idx + 4;   
			}
			// Read from SRAM and compare data
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, 0, 2);	// param3:2, Word Write
			printf("PASS\n");
		}else
		{
			// Unfixed data pattern
			// Write to SRAM
			u32Idx = 0;
			u32WriteData = 0;
			pu32DevPtr = (uint32_t *)EBI_START_BASE_ADDR;
			printf("	0x0, 0x1, 0x2, 0x3, 0x4 ... Access with offset(0x%X) ... ", (uint32_t)&u32Idx);
			while (u32Idx < u32EBIsize)					
			{
				*pu32DevPtr++ = (uint32_t)(u32WriteData + (uint32_t)&u32Idx);
				u32Idx = u32Idx + 4;   
				u32WriteData++;
			}
			SRAM_CompareFunction(u32DataIdx, u32EBIsize, (uint32_t)&u32Idx, 2);	// param3:2, Word Write
			printf("PASS\n\n");	
		}
		u32DataIdx++;
	}
}

