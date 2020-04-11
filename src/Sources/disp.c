
#include "extern_port.c"
#include "extern_var.c"

extern void Write_line(char,char *);
extern void Write_string(char,char *);
extern void Write_lcd_comm(char);
extern void Write_lcd_data(char);



//									      {"0123456789abcdef"}		
#pragma CONST_SEG ROM
#define DEF_OPT3 "OPT3   "
#define DEF_HALF "H/F CUP"
#define DEF_BREW_ON "ON     "
#define DEF_BREW_OFF "OFF    "

unsigned char const Msg_para[MAX_PARA+1][16] = 
										{
										  {"Button C=       "},
										  {"Premix  A =    s"},
//									      {"Premix2 A =    s"},      
//									      {"Premix3 A =    s"},      
									      {"Water   A =    s"},      
//									      {"Water2  A =    s"},      
//								          {"Water3  A =    s"},      

										  {"Premix  B =    s"},
//									      {"Premix2 B =    s"},      
//									      {"Premix3 B =    s"},      
									      {"Water   B =    s"},      
//									      {"Water2  B =    s"},      
//								          {"Water3  B =    s"},      
									  
									      {"Premix  C =    s"},
//									      {"Premix2 C =    s"},      
//									      {"Premix3 C =    s"},      
									      {"Water   C =    s"},      
//									      {"Water2  C =    s"},      
//								          {"Water3  C =    s"},
								          
								          {"WATER1  D =    s"},
								          {"WATER2  D =    s"},
								          {"WATER3  D =    s"},
								     
								          {"Refil Time=    s"},
								          {"Flush Time=    s"},
								          {"Flush TO  =    m"},
								          {"Delay Time=    s"},      
//										  {"Msg for Opt A   "},
//										  {"Msg for Opt B   "},
//										  {"Msg for Opt C   "}
										  {"Brew     		"},
										  {"Brew Time       "}
										   								    									   	        
										};

unsigned char const Option_cntr[MAX_OPTION][16]=
										{
										  {"OPT A =         "},
										  {"OPT B =         "},
										  {"OPT C =         "},
										  {"OPT D =         "},
										};  
									
//unsigned char const Message_arr[MAX_MSG][16]=
//									{
//										  {"DEFAULT         "},
//
 //										  {"TEA             "},
//										  {"TEA REGULAR     "},
//										  {"TEA W/O MILK    "},
//										  {"TEA W/O SUGAR   "},
//										  
//										  {"COFFEE          "},
//										  {"COFFEE REGULAR  "},
//										  {"COFFEE W/O MILK "},
//										  {"COFFEE W/O SUGAR"},
//										  
//										  {"SOUP            "},
//									};
										  
//										  
// Displays the message 
//void Display_msg(void)
//{
//	Write_line(0,&Message_arr[Msg_arr_index][0]);
//}	



/*****************Disp_Para*******************/
// If parameter = 1 then display Premix1
// If parameter = 2 then display Premix2
void Disp_parameter(void)
{
	Write_line(0,&Msg_para[Parameter][0]);
}



void Disp_option(void)
{
	Write_line(0,&Option_cntr[Counter-1][0]);
}


/*****************Bin2ascii*******************/
// Converts Binary value to Ascii value for displaying on LCD

void Bin2ascii(unsigned char val)
{
	Ascii_hun = (val / 100) + 0x30;
	val = val % 100;
	Ascii_ten = (val / 10) + 0x30;
	val = val % 10;
	Ascii_unit = val + 0x30;
}


/*****************Disp_para_val***************/
// Displays the value on LCD which are read from memory
void Disp_para_val(void)
{
	Write_lcd_comm(PARA_VAL_ADD);
	
	if(Parameter == DEF_HALF_MODE)
	{
		if(Parameter_val[DEF_HALF_MODE]==1)
		{
			Write_string((PARA_VAL_ADD - 2),DEF_HALF);
		}
		else
		{
			Write_string((PARA_VAL_ADD - 2),DEF_OPT3);
		}
	}
/*	else if(((Parameter-1)>=DEF_MSG_SETTING_MIN) && ((Parameter-1)<=DEF_MSG_SETTING_MAX)) 
	{
		Display_msg();
	}
*/	
	else if((Parameter)==DEF_BREW_SETTING)
	{
		if(Parameter_val[DEF_BREW_SETTING]==1)
		{
			Write_string((PARA_VAL_ADD - 2),DEF_BREW_ON);
		}
		else
		{
			Write_string((PARA_VAL_ADD - 2),DEF_BREW_OFF);
		}
	}	
	else
	{
		Bin2ascii(Parameter_val[Parameter]);		
		Write_lcd_data(Ascii_hun);
		Write_lcd_data(Ascii_ten);
	
		if (Parameter != DEF_FLUSH_TOUT)
			Write_lcd_data('.');
			
		Write_lcd_data(Ascii_unit);
	}
}

/****************Disp_cup_status***************/
// Displays the status of CUP on LCD i.e. HALF CUP OR FULL CUP

void Disp_cup_status()
{
	Initlcd();
	Write_line(0,"READY           ");	
}

	
/*
// This function is called in Setting Mode for Flush Time Out as it is in minutes

void Disp_flush_timeout(void)
{
	Write_lcd_comm(PARA_VAL_ADD);
	Bin2ascii(Parameter_val[Parameter-1]);		
	Write_lcd_data(Ascii_hun);
	Write_lcd_data(Ascii_ten);
	Write_lcd_data(Ascii_unit);
}
*/

