#include "jl3.h"
#include "extern_var.c"
#include "extern_port.c"

void Gen_start(void);
void Gen_stop(void);                       
void Write_iic_byte(unsigned char, unsigned char);
unsigned char Read_iic_byte(unsigned char);
void Delay_small(void);
void Tx_eight_bits(unsigned char);
unsigned char Pulse_scl(void);
unsigned char Byte_iic(unsigned char);


unsigned char Pulse_scl(void)
{
	unsigned char Input_val = 0;

	Delay_small();
	Scl = 1;
	Delay_small();
	if(Sda)
		Input_val = 1;	
	else
		Input_val = 0;
	
	Scl = 0;
	Delay_small();
	return(Input_val);
	
}

void Delay_small(void)
{
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;

}

void Gen_start(void)
{
	SDA_DDR |= SDA_BIT;
	Sda = 1;
	Delay_small();
	Scl = 1;
	Delay_small();
	Sda = 0;
	Delay_small();
	Scl = 0;

}

void Gen_stop(void)
{
	Sda = 0;
	SDA_DDR |= SDA_BIT;
	Delay_small();
	Scl = 1;
	Delay_small();
	Sda = 1;	

}

void Tx_eight_bits(unsigned char Data1)
{
	for(Tmp_cntr_iic = 0; Tmp_cntr_iic <8; Tmp_cntr_iic++)
	{
		SDA_DDR |= SDA_BIT;
		if (Data1 & 0x80)
			Sda = 1;
		else
			Sda = 0;

		Data1 <<= 0x01;
		Pulse_scl();
	}

}

unsigned char Byte_iic(unsigned char Data)
{
//	unsigned char temp;
	Tx_eight_bits(Data);
	SDA_DDR &= (255 - SDA_BIT);
	Temp = Pulse_scl();
	Sda = 0;
	return Temp;
}

void Write_iic_byte(unsigned char Eep_add, unsigned char Tx_byte)
{
	static unsigned char Restart_cntr;
	Iic_buff[0] = 0xa0;
	Iic_buff[1] = Eep_add;
	Iic_buff[2] = Tx_byte;
	
Restart_tx:	

	if(Restart_cntr > 10)
	{	
/* If the mcu is unable to communicate with IIC devide for more than 10 times, then clear 
counter	and return. Set Iic_bad flag
*/
		Restart_cntr = 0;
		Iic_bad = 1;
		Gen_stop();
		return;
	}
	
	Gen_start();
	for(Iic_counter = 0; Iic_counter < 3; Iic_counter++)
	{
		if(Byte_iic(Iic_buff[Iic_counter]))
		{
			Restart_cntr++;
			goto Restart_tx;
		}
	}
	Gen_stop();
	/* On every successfull write, clear restart cntr and iic bad flag */
	
	Restart_cntr = 0;
	Iic_bad = 0;
}

unsigned char Read_iic_byte(unsigned char Eep_add)
{
	static unsigned char Restart_cntr;
	Iic_buff[0] = 0xa0;
	Iic_buff[1] = Eep_add;

Restart_rx:
	
	Restart_cntr++;
	if(Restart_cntr > 10)
	{
		/* If the mcu is unable to communicate with IIC devide for more than 10 times, then clear counter
			and return. Set Iic_bad flag
		*/
		Restart_cntr = 0;
		Iic_bad = 1;
		Gen_stop();
		return;
	}
		
	
	Gen_start();
	if(Byte_iic(Iic_buff[0]))
		goto Restart_rx;
	
	if(Byte_iic(Iic_buff[1]))
		goto Restart_rx;
	
	Gen_start();
	
	if(Byte_iic((Iic_buff[0]+1)))
		goto Restart_rx;

	for(Tmp_cntr_iic = 0; Tmp_cntr_iic < 8; Tmp_cntr_iic++)
	{
		Iic_buff[3] <<= 0x01;
		Iic_buff[3] = Iic_buff[3] | Pulse_scl();
	}
	Gen_stop();

	/* On every successfull read, clear restart cntr and iic bad flag */
	Restart_cntr = 0;
	Iic_bad = 0;

	return Iic_buff[3];	
}

void Chk_eeprom(void)
{
	/* Routine to test EEPROM and set a bit Mem_bad if EEPROM does not respomd.
		Generate start condition
		Send address of EEPROM
		If ack not received
			Set bit Mem_bad
	*/			

	Mem_bad = 0;
	Gen_start();
	if(Byte_iic(0xA0))
		Mem_bad = 1;
	Gen_stop();		
}

/*
void Iic_tx(void)
{
Restart_iic_tx:
	Gen_start();
	for(Iic_counter=0; Iic_counter <  Max_iic_bytes; Iic_counter++)
	{
		if(Byte_iic(Iic_buff[Iic_counter]))
		{
			goto Restart_iic_tx;
		}
	}
	Gen_stop();
}

void Iic_rx(void)
{
	SDA_DDR &= (255 - SDA_BIT);	
	for(Iic_counter = 0; Iic_counter < 8; Iic_counter++)
	{
		Iic_buff[3] <<= 0x01;
		Iic_buff[3] = Iic_buff[3] | Pulse_scl();
	}
}
*/

