  #pragma ONCE
  #define MC68HC908JL3

  /*  IO DEFINITIONS FOR MC68HC908JL3 - rev 3.0 */


  /*  PORTS section */

  #define PTA   (*(volatile unsigned char*)(0x00))  /* port A */
  #define pPTA   (volatile unsigned char*)(0x00)    /* port A */
  
  #define PTB   (*(volatile unsigned char*)(0x01))  /* port B */
  #define pPTB   (volatile unsigned char*)(0x01)    /* port B */
  
  #define PTD   (*(volatile unsigned char*)(0x03))  /* port D */
  #define pPTD   (volatile unsigned char*)(0x03)    /* port D */
  
  #define DDRA  (*(volatile unsigned char*)(0x04))  /* data direction port A */
  #define pDDRA  (volatile unsigned char*)(0x04)    /* data direction port A */
  
  #define DDRB  (*(volatile unsigned char*)(0x05))  /* data direction port B */
  #define pDDRB  (volatile unsigned char*)(0x05)    /* data direction port B */
  
  #define DDRD  (*(volatile unsigned char*)(0x07))  /* data direction port D */
  #define pDDRD  (volatile unsigned char*)(0x07)    /* data direction port D */
   
  #define PDCR  (*(volatile unsigned char*)(0x0A))  /* Port D control register */
  #define pPDCR  (volatile unsigned char*)(0x0A)    /* Port D control register */
  
  #define PTAPUE  (*(volatile unsigned char*)(0x0D))  /* Port A input pullup Enable */
  #define pPTAPUE  (volatile unsigned char*)(0x0D)    /* Port A input pullup Enable */
  
 
  /* KEYBOARD section */

  #define KBSCR   (*(volatile unsigned char*)(0x1A))   /* Keyboard status and control register */
  #define pKBSCR    (volatile unsigned char*)(0x1A)    /* Keyboard status and control register */
  
  #define KBIER    (*(volatile unsigned char*)(0x1B))  /* Keyboard interrupt Enable */
  #define pKBIER    (volatile unsigned char*)(0x1B)    /* Keyboard interrupt Enable */
  
 

  /*  INTERRUPT section */
  
  #define ISCR   (*(volatile unsigned char*)(0x1D)) /* IRQ control/status register */ 
  #define pISCR   (volatile unsigned char*)(0x1D)   /* IRQ control/status register */ 
  
  
  /* CONFIGURATION section */

  #define CONFIG2   (*(volatile unsigned char*)(0x1E)) /* Configuration register 2 */ 
  #define pCONFIG2   (volatile unsigned char*)(0x1E)   /* Configuration register 2 */ 

  #define CONFIG1   (*(volatile unsigned char*)(0x1F)) /* Configuration register 1 */ 
  #define pCONFIG1   (volatile unsigned char*)(0x1F)   /* Configuration register 1 */ 
  
  
  /*  A TIMER section */

  #define TSC    (*(volatile unsigned char*)(0x20)) /* timer A status/ctrl register */  
  #define pASC    (volatile unsigned char*)(0x20)   /* timer A status/ctrl register */  
  
 
  #define TCNTHL (*(volatile unsigned short*)(0x21)) /* timer A counter high and low */
  #define pTCNTHL (volatile unsigned short*)(0x21)   /* timer A counter high and low */
  
  #define TCNTH  (*(volatile unsigned char*)(0x21)) /* timer A counter high */
  #define pACNTH  (volatile unsigned char*)(0x21)   /* timer A counter high */
  
  #define TCNTL  (*(volatile unsigned char*)(0x22)) /* timer A counter low */   
  #define pTCNTL  (volatile unsigned char*)(0x22)   /* timer A counter low */ 
  
  #define TMODHL (*(volatile unsigned short*)(0x23)) /* timer A modulo high and low */
  #define pTMODHL (volatile unsigned short*)(0x23)   /* timer A modulo high and low */

  #define TMODH  (*(volatile unsigned char*)(0x23)) /* timer A modulo high */  
  #define pTMODH  (volatile unsigned char*)(0x23)   /* timer A modulo high */
  
  #define TMODL  (*(volatile unsigned char*)(0x24)) /* timer A modulo low */  
  #define pTAMODL  (volatile unsigned char*)(0x24)   /* timer A modulo low */
  
  #define TSC0   (*(volatile unsigned char*)(0x25)) /* timer A channel 0 status/ctrl */   
  #define pTSC0   (volatile unsigned char*)(0x25)   /* timer A channel 0 status/ctrl */ 
  
  #define TCH0HL (*(volatile unsigned short*)(0x26)) /* timer A channel 0 high and low*/
  #define pTCH0HL  (volatile unsigned short*)(0x26)   /* timer A channel 0 high and low*/  
  
  #define TCH0H  (*(volatile unsigned char*)(0x26)) /* timer A channel 0 high */  
  #define pTCH0H  (volatile unsigned char*)(0x26)   /* timer A channel 0 high */
  
  #define TCH0L  (*(volatile unsigned char*)(0x27)) /* timer A channel 0 low */  
  #define pTCH0L  (volatile unsigned char*)(0x27)   /* timer A channel 0 low */
  
  #define TSC1   (*(volatile unsigned char*)(0x28)) /* timer A channel 1 status/ctrl */    
  #define pTSC1   (volatile unsigned char*)(0x28)   /* timer A channel 1 status/ctrl */  
  
  #define TCH1HL (*(volatile unsigned short*)(0x29)) /* timer A channel 1 high and low*/
  #define pTCH1HL (volatile unsigned short*)(0x29)   /* timer A channel 1 high and low*/

  #define TCH1H  (*(volatile unsigned char*)(0x29)) /* timer A channel 1 high */  
  #define pTCH1H  (volatile unsigned char*)(0x29)   /* timer A channel 1 high */
  
  #define TCH1L  (*(volatile unsigned char*)(0x2A)) /* timer A channel 1 low */  
  #define pTCH1L  (volatile unsigned char*)(0x2A)   /* timer A channel 1 low */
  
  
  /*  A/D section */

  #define ADSCR   (*(volatile unsigned char*)(0x3c)) /* ADC status and control register */
  #define pADSCR   (volatile unsigned char*)(0x3c)   /* ADC status and control register */

 	
  #define ADR    (*(volatile unsigned char*)(0x3d)) /* ADC data register */  
  #define pADR    (volatile unsigned char*)(0x3d)   /* ADC data register */
  
  #define ADCLK   (*(volatile unsigned char*)(0x3e)) /* ADC clock register */
  #define pADCLK   (volatile unsigned char*)(0x3e) /* ADC clock register */

 
  /* SIM */
    
  #define SBSR   (*(volatile unsigned char*)(0xfe00))  /* SIM break status register */
  #define pSBSR   (volatile unsigned char*)(0xfe00)    /* SIM break status register */
  
  #define SRSR   (*(volatile unsigned char*)(0xfe01))  /* SIM reset status register */
  #define pSRSR   (volatile unsigned char*)(0xfe01)    /* SIM reset status register */
  
  #define SBFCR  (*(volatile unsigned char*)(0xfe03))  /* SIM break flag control register */
  #define pSBFCR  (volatile unsigned char*)(0xfe03)    /* SIM break flag control register */
  
  #define INT1  (*(volatile unsigned char*)(0xfe04))  /* Interrupt status register 1 */
  #define pINT1  (volatile unsigned char*)(0xfe04)    /* Interrupt status register 1 */
  
  #define INT2  (*(volatile unsigned char*)(0xfe05))  /* Interrupt status register 2 */
  #define pINT2  (volatile unsigned char*)(0xfe05)    /* Interrupt status register 2 */
  
  #define INT3  (*(volatile unsigned char*)(0xfe06))  /* Interrupt status register 3 */
  #define pINT3  (volatile unsigned char*)(0xfe06)    /* Interrupt status register 3 */

    
  /* FLASH section*/

  #define FLCR    (*(volatile unsigned char*)(0xfe08))  /* Flash control register */  
  #define pFLCR    (volatile unsigned char*)(0xfe08)    /* Flash control register */

  #define FLBPR    (*(volatile unsigned char*)(0xfe09))  /* Flash Block protect register */  
  #define pFLBPR    (volatile unsigned char*)(0xfe09)    /* Flash Block protect register */
  
  /*BREAK MODE*/
  
  #define BRKH   (*(volatile unsigned char*)(0xfe0c))  /* SIM reset status register */
  #define pBRKH   (volatile unsigned char*)(0xfe0c)    /* SIM reset status register */
  
  #define BRKL   (*(volatile unsigned char*)(0xfe0d))  /* SIM reset status register */
  #define pBRKL   (volatile unsigned char*)(0xfe0d)    /* SIM reset status register */
  
  #define BRKSCR (*(volatile unsigned char*)(0xfe0e))  /* SIM reset status register */  
  #define pBRKSCR (volatile unsigned char*)(0xfe0e)    /* SIM reset status register */


  #define COPCTL (*(volatile unsigned char*)(0xffff))  /* COP control register */  
  #define pCOPCTL (volatile unsigned char*)(0xffff)    /* COP control register */
  
  
