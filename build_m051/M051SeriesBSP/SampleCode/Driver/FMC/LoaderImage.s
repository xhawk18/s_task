;/*---------------------------------------------------------------------------------------------------------*/
;/*                                                                                                         */
;/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
;/*                                                                                                         */
;/*---------------------------------------------------------------------------------------------------------*/


    AREA _image, DATA, READONLY

    EXPORT  loaderImageBase
    EXPORT  loaderImageLimit
    
    ALIGN   4
        
loaderImageBase
    INCBIN .\obj\FMC_LD.bin
loaderImageLimit

    
    END