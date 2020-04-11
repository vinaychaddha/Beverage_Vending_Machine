
#include "extern_port.c"
#include "extern_var.c"

#define LCD_SINGLE_LINE
void Pulse_E(void);
void Write_lcd_comm(unsigned char);
void Delay_ticks(unsigned int);
/* 
The following pragma (CONST_SEG) will tell the compiler to place the constants in ROM. 
Otherwise, compiler places the constants in RAM
*/

#pragma CONST_SEG ROM
const unsigned char lcd_table[5] = {0x28, 0x0C, 0x01, 0x06, 0x14};
const unsigned char lcd_initable[4] = {0x03, 0x03, 0x03, 0x02};


void Pulse_E(void)
{
	
	Lcd_e = 1;
	asm nop;
	asm nop
	asm nop
	asm nop
	asm nop
	asm nop
	Lcd_e = 0;
}


/*
	Function to initialise the LCD.
	Inputs			: None
	Outputs			: Initialisation of LCD
	Routines called	: Tx_LNibble, Write_Lcd_Comm, Delay
*/

void Write_lcd_comm(unsigned char val)
{

	Lcd_rs = 0;

	Lcd_data = ((val & 0xF0)>>4);
	Pulse_E();	

	Lcd_data = (val & 0x0F);
	Pulse_E();	
		
}

/*
	Function to write a data byte on LCD.
	Inputs			: Value of the data to be written
	Outputs			: Data displayed on LCD
	Routines called	: Tx_LNibble, Tx_UNibble, Delay
*/

void Write_lcd_data (unsigned char val)
{

	Lcd_rs = 1;

	Lcd_data = ((val & 0xF0)>>4);
	Pulse_E();	

	Lcd_data = (val & 0x0F);
	Pulse_E();	

}


void Initlcd(void)
{

/*
To initialise LCD:
	- Set RS Low
	- Transmit lower nibbles of 4 bytes from LCD_INITABLE array
	- Transmit 5 bytes from LCD_TABLE array
*/
	
	Lcd_rs = 0;

	for (Lcd_cntr = 0; Lcd_cntr < 4; Lcd_cntr++)	/* Loop to tx 4 nibbles*/
	{

		Lcd_data = (lcd_initable[Lcd_cntr] & 0x0F);
		Pulse_E();	
		Delay_ticks(1);		
	}

	for (Lcd_cntr = 0; Lcd_cntr < 5; Lcd_cntr++)	/* Loop to tx 5 bytes*/
	{
		Write_lcd_comm (lcd_table[Lcd_cntr]);
		Delay_ticks(1);		
	}

}
													
													
/*
	Function to write a command on LCD.
	Inputs			: Value of the command to be written
	Outputs			: Command output to LCD
	Routines called	: Tx_LNibble, Tx_UNibble, Delay
*/




void Write_line(unsigned char line_no, unsigned char * msg_add)
{
	if(line_no == 0)
		Write_lcd_comm(0x80);
	else if(line_no == 1)
		Write_lcd_comm(0xC0);

#ifdef LCD_FOUR_LINE
	
	else if(line_no == 2)
		Write_lcd_comm(0x90);
	else if(line_no == 3)
		Write_lcd_comm(0xD0);
		
#endif
		
	for(Lcd_pointer=0; Lcd_pointer < 16; Lcd_pointer++)
	{

#ifdef LCD_SINGLE_LINE
		if(Lcd_pointer == 8)
			Write_lcd_comm(0xC0);
		
#endif
		Write_lcd_data(*msg_add);
		msg_add++;

	}
}

void Write_string(unsigned char Start_add, unsigned char * Msg_add)
{
	Write_lcd_comm(Start_add);
	for(Lcd_pointer=0; Lcd_pointer < 16; Lcd_pointer++)
	{
		Write_lcd_data(*Msg_add);
		Msg_add++;
		if(*Msg_add == '\0')
			break;

	}
	
}

void Delay_ticks(unsigned int dly)
{
	One_tick_over=0;
	while(dly!=0)
	{
		if(One_tick_over)
		{
			One_tick_over=0;
			dly--;
		}
	}
}
