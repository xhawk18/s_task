;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/


    AREA _image, DATA, READONLY

    EXPORT  g_u32LoaderImageBase
    EXPORT  g_u32LoaderImageLimit
    
    ALIGN   4
        
g_u32LoaderImageBase
    INCBIN .\obj\FMC_LD.bin
g_u32LoaderImageLimit

    
    END