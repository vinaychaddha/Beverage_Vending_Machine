#include "jl3.h"

//This pragms disables the warning message caused by structure definitions using unsigned char
//in place of unsigned int
#pragma MESSAGE WARNING DISABLE C1106

/*-------------- PortA definition -----------------------------------------------

PORTA 		7		6		5		4		3		2		1		0	
			-		-		RLY     RLY     RLY    RLY     RLY	 	KBD
def_ddra	1		1		1		1		1		1		1		0
def_porta	0       0       0       0       0       0       0       0 
---------------------------------------------------------------------------------*/

#define Def_ddra  0xfe
#define Def_porta 0x00	

extern volatile union {
	struct	{
		unsigned char Kbd:1;
		unsigned char Rly_premix1:1;
		unsigned char Rly_water1:1;
		unsigned char Rly_premix2:1;
		unsigned char Rly_water2:1;
		unsigned char Rly_premix3:1;
	}PORTA_BITS;	
	unsigned char Porta_port;
} sPTA @ 0x00;


#define  Kbd 			sPTA.PORTA_BITS.Kbd
#define  Rly_premix1	sPTA.PORTA_BITS.Rly_premix1
#define  Rly_water1		sPTA.PORTA_BITS.Rly_water1
#define  Rly_premix2	sPTA.PORTA_BITS.Rly_premix2
#define  Rly_water2		sPTA.PORTA_BITS.Rly_water2
#define  Rly_premix3	sPTA.PORTA_BITS.Rly_premix3


/*-------------- PortB definition -------------------------------------------------
portb		7		6		5		4		3		2		1		0
			D7      D6      D5      D4      E       RS     SDA      SCL
def_ddrb	1		1		1		1		1		1		1		1		
def_portb	0		0		0		0		0		0		1		1
-----------------------------------------------------------------------------------*/

#define Def_ddrb   0xff
#define Def_portb  0x03

extern volatile union {
	struct	{
				
				unsigned char Scl:1;
				unsigned char Sda:1;
				unsigned char Lcd_rs:1;
				unsigned char Lcd_e:1;		
				unsigned char Lcd_data:4;
	}PORTB_BITS;	
	unsigned char Portb_port;
} sPTB  @ 0x01;


#define Scl			sPTB.PORTB_BITS.Scl
#define Sda         sPTB.PORTB_BITS.Sda         			
#define SDA_DDR		DDRB
#define SDA_BIT		0x02
#define Lcd_e		sPTB.PORTB_BITS.Lcd_e
#define Lcd_rs		sPTB.PORTB_BITS.Lcd_rs
#define Lcd_data	sPTB.PORTB_BITS.Lcd_data


/* --------------- PortD defination -------------------------------------
portd		7		6		5		4		3		2		1		0
			- 		-       -   	-       -      RLY      RLY    RLY  
def_ddrd    0		0		0  		0		0      	1      	1	    1
def_portd   0		0       0       0       0    	0      	0       0
-------------------------------------------------------------------------*/

#define Def_ddrd 0x07
#define Def_portd 0x00

extern volatile union {
	struct	{
				unsigned char Rly_water3:1;
				unsigned char Rly_refil:1;
			}PORTD_BITS;	
			unsigned char Portd_port;
			
} sPTD @ 0x03;

#define Rly_water3	sPTD.PORTD_BITS.Rly_water3
#define Rly_refil	sPTD.PORTD_BITS.Rly_refil

