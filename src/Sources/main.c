
/* 
  Aug 14th , 2006         Day - Monday
  One problem found that unit is not working if we program same software in JL8 and JL16.
  The bug was that the RAM address is defined in the program as 80h which is a starting address for JL3 controller.
  The starting RAM address for JL8 and JL16 is 60h. Therefore in the program while cold boot RAm gets clear starting 
  from 80h.And RAM variables from 60h to 7fh remains in the random state.
  So one constant is defined in the main.c file.
  #define RAM_START_ADD   0x60
  In the startup file Init() function is made comment so that program will
  not clear RAM automatically.
     

	July 24,2003
	1. initlcd done along with display of ready message
	   this is done to ensure auto display correction when error occurs
	   ec to be released after sample works okay in field
       change done by vinay chaddha
       
    2. Manufacturer's message added on startup for half second
       change done by Pravesh
       
   	3. Option messages changed to Serving Option 1, Serving Option 2, Serving Option 3, 
   	   and Hot Water.
   	   change done by pravesh
   	   

	PROJECT TEA COFFEE MACHINE WITH BREWING OPTION 
	STARTED 21 MARCH 2003 
	
TWO OPTIONS FIXED(COFFEE, TEA), THIRD BUTTON (OPTION3(SOUP) OR HALF/FULL CUP), FOURTH BUTTON FOR HOT WATER
BREW MODE AND BREW TIME, FLUSH TIME, DELAY, FLUSH TIMEOUT,REFIL TIME
	
FIRST DEMO ON 24 MARCH 2003

AFTER BD25-03-2003 10-20 AM
---------------------------
CHANGES
-------

1. (PARAMETER-1) REPLACED BY (PARAMETER) 					-		 DONE
2. MAXIMUM TIME LIMIT SET TO 9.9s  EXCEPT FOR REFIL TIME 	-		 DONE
3. KEY1 IN SETTING MODE USED TO SET THE VALUE 0 			-		 DONE



AFTER FOURTH DEMO CHANGES REQUIRED BY MR. SAHUL KHURANA
-------------------------------------------------------
AFTER BD 28-03-2003 1-30PM
--------------------------
CHANGES
-------

1. PRE BREW TIME SET TO 30% OF WATER TIMEOUT - DONE 
2. IF BREW MODE OFF THEN BREW TIME WILL NOT BE TAKEN - DONE 
3. BUG WAS FOUND IN BREWING MODE WHEN BREW MODE WAS ON AND BREW TIME WAS SET TO 0.0 THEN
   MACHINE DID NOTHING AND AFTER EVERY FLUSH TIMEOUT ASKED FOR FLUSH
  
 STATUS FOR BUG - REMOVED 



*/ 

#include "variables.c"
#include "port.c"

#define RAM_START_ADD   0x60

#pragma CONST_SEG ROM
unsigned char const Memory_msg[] = {"MEMORY BAD      "};
unsigned char const Setting_msg[]= {"SETTING MODE    "};
unsigned char const Counter_msg[]= {"COUNTER MODE    "};
//unsigned char const Serving_opt1_msg[]={"COFFEE          "};
//unsigned char const Serving_opt2_msg[]={"TEA             "};
//unsigned char const Serving_opt3_msg[]={"SOUP            "};
unsigned char Temp_water_timeout, Temp_premix_timeout;

unsigned char const Serving_opt1_msg[]={"SERVING OPTION 1"};
unsigned char const Serving_opt2_msg[]={"SERVING OPTION 2"};
unsigned char const Serving_opt3_msg[]={"SERVING OPTION 3"};
unsigned char const Serving_opt4_msg[]={"HOT WATER       "};
unsigned char const Flush_msg[]       ={"FLUSH REQUIRED  "};
unsigned char const Flushing_msg[]    ={"FLUSHING        "};
unsigned char const Mfr_message[]     ={"24-07-03 JL3 2/3"};

extern void Init_timer(void);
extern void Initlcd(void);
extern void Setime(void);
extern void Write_lcd_comm(unsigned char);
extern void Write_lcd_data(unsigned char);
extern void Write_line(unsigned char,unsigned char *);
extern void Write_string(unsigned char,unsigned char *);
extern void Sense_kbd(void);
extern void Disp_parameter(void);
extern char Read_iic_byte(char);
extern void Write_iic_byte(char,char);
extern void Disp_para_val(void);
extern unsigned char Chk_dec_timeout(unsigned char *);
extern void Chk_eeprom(void);
extern void Delay_small(void);
extern void Disp_option(void);
extern void Disp_cup_status(void);
//extern unsigned char const Message_arr[MAX_MSG][16];
extern unsigned char const Option_cntr[MAX_OPTION][16];
void Act_key1(void);			// Action Definition for KEY1
void Act_key2(void);			// Action Definition for KEY2
void Act_key3(void);			//Action definition for KEY3
void Act_key4(void);			// Action definition for KEY4
void Chk4poweron_key(void);				// Implements checks on Power ON whether to switch on to which mode
void Initports(void);
void Bin2ascii(char);					// Converts a binary value to ascii value
void Switch_on_relay(void);				// Switches the relay ON 
void Switch_off_relay(void);			// Switches the relay OFF
void Read_all_para(void);				// Reads all the timeout values in memory 
void Set_time_out(void);				// sets the values fir time outs as read by memory into parameter_val array
void Check_n_set_refil_relay(void);		// To switch on Refill Relay
void Read_counter_val(void);
void Display_counter_val(void);
void Update_counter_val(void);
void Write_counter_val(void);
void Set_flush_timeout(void);
void Set_half_opt(void);
void Set_brew_mode(void);			// To set brew on/off
void Previous_state(void);

void main(void) 
{
	unsigned char I;
	CONFIG1	= 0x3;   						// Disable watch dog
	CONFIG2 = 0x10;							// brownout enable   						// Disable watch dog
	Initports();							// Initialise all ports
	Init_timer();		
	Initlcd();
	I = SRSR;						// Check for power on reset
	I &= 0x80;						// check 7 th bit of SRSR (Reset status register)

	if((I!=0) && (Test_pattern!=0x55aa))
	{		
	/*  Clear RAM location */
		 asm clrh;  
		 asm clrx;
		 asm ldx	#RAM_START_ADD;		// here ram variables from 60 to ff will be reset.
	asm nxt_mem: 
		 asm clr 	,x;
		 asm incx;
		 asm bne	nxt_mem;
			 
			
		Test_pattern = 0x55aa;		// Set test pattern to default value when cold boot.
		Chk4poweron_key();
		Chk_eeprom();							// Checks Memory Integrity
		if(Mem_bad)
		{
			Write_line(0,&Memory_msg);
			while(1);
		}
		else if(Setting_mode==0 && Counter_mode==0)
		{
			Write_line(0,&Mfr_message);
			Delay_ticks(100);
			Disp_cup_status();
			Read_all_para();				// Reads all parameter values in Parameter_val Array
			Rly_on_dly_tout=RLY_ON_DLY_TICKS;    // set rly dly time to sw on each rly after
												// some delay
			Flush_timeout = Parameter_val[DEF_FLUSH_TOUT];
		}
		Parameter = 0xff;				// Initially set parameter to ff so that on first key press
															// unit will set pointer to 0
	}
	else
	{
		Previous_state();
	}
	
	if(!Mem_bad)
	{
		while(1)	
		{		
			if(One_tick_over)
			{
				One_tick_over=0;
				Setime();
				Sense_kbd();
			
				if(New_key_found)
				{
					New_key_found=0;
					switch(Kbd_pos)
					{
						case KEY1:
								Act_key1();
								break;
						case KEY2:
								Act_key2();	
								break;
						case KEY3:
								Act_key3();	
								break;
						case KEY4:
								Act_key4();							
					}					
				}

				if(Chk_dec_timeout(&Rly_on_dly_tout))
				{
					Rly_on_dly_tout=RLY_ON_DLY_TICKS;				
					Switch_on_relay();			// on relay one by one after n msec delay
				}
			}

			// After every 100 ms checks for the timeouts for all premixes 
			// and calls Switch_Relay() to switch the corresponding relay ON/OFF			
			// Dispense = 1 when machine is serving
			// Dispense =0 when Machine in Ready State
			if(Ms100_over)
			{
				Ms100_over = 0;
										
				if(Delay_time == 0)
				{
/*					if(Chk_dec_timeout(&Premix1_timeout))
						Switch_off_relay();
					if(Chk_dec_timeout(&Premix2_timeout))
						Switch_off_relay();
					if(Chk_dec_timeout(&Premix3_timeout))
						Switch_off_relay();
*/	
					if(Chk_dec_timeout(&Premix_timeout))
					{
						Switch_off_relay();
					}
				}
				if(Chk_dec_timeout(&Water_timeout))
					Switch_off_relay();
				if(Chk_dec_timeout(&Water1_timeout))
					Switch_off_relay();
				if(Chk_dec_timeout(&Water2_timeout))
					Switch_off_relay();
				if(Chk_dec_timeout(&Water3_timeout))
					Switch_off_relay();
				if(Chk_dec_timeout(&Refil_timeout) && (Brewing==0))
				{
					Switch_off_relay();
					Dispensing = 0;
					Hot_water_reqd = 0;
					Flushing =0;
					Disp_cup_status();
					// Set Delay_time to the value set 
					Delay_time = Parameter_val[DEF_DELAY_TIME];
				}
				if((Parameter_val[DEF_BREW_SETTING]==1) && (Water_timeout==0) && (Counter == OPTION2))
				{
					if(Chk_dec_timeout(&Brew_time))
					{
						Brewing = 0;
						Delay_time = Parameter_val[DEF_DELAY_TIME];
						Premix_timeout = Temp_premix_timeout;
						Water_timeout = Temp_water_timeout - Pre_brew_time;
					}
				}	
				// checks for timeouts of all premixes and water outlets 
				// If all Over then SWITCHES ON REFIL relay in order to 
				// refil water
				Check_n_set_refil_relay();
				Chk_dec_timeout(&Delay_time);
			}
	
			if(One_min_over)
			{
				One_min_over = 0;
				if(Chk_dec_timeout(&Flush_timeout))
				{
					Flush_reqd = 1;
					Write_line(0,&Flush_msg);
				}
			}	
		}
	}	
}

//******************** ACT KEY1 ****************************
// Program comes here when key1 is pressed.
// Do not act upon this key if pressed in setting mode.

void Act_key1(void)
{
	unsigned char i;
	if(Counter_mode)
		return;
	else if(Setting_mode)
	{
		if(Parameter!=0xff)
		{
			if(Parameter==DEF_HALF_MODE)
			{
				// In half cup mode do nothing 
			}
			
			if(Parameter==DEF_BREW_SETTING)
			{
				// Do nothing
			}

/*			else if(((Parameter)>=DEF_MSG_SETTING_MIN) && ((Parameter)<=DEF_MSG_SETTING_MAX)) 	// for message setting mode
	 		{
				if(Msg_arr_index==MAX_MSG-1)
					Msg_arr_index = MIN_MSG-1;
				else
					Msg_arr_index++;
				Parameter_val[Parameter]=Msg_arr_index;

				// In message setting mode do nothing
			}
*/			else
			{
				Parameter_val[Parameter]=0;	
				if((Parameter)==DEF_FLUSH_TOUT)
				{
					if(Parameter_val[Parameter]>99)
						Parameter_val[Parameter] = 0 ;
				}
			}
			Disp_para_val();	
		}
	}
	else
	{
		if(Flush_reqd)
		{
//			Write_line(0,&Flush_msg);		
			return;
		}
		else if (Dispensing ==0)		// If not Busy Serving
		{
			/* Start UPDATING Counter */
			Counter = OPTION1;
			Update_counter_val();
			/* END UPDATING Counter  */
//			if(Parameter_val[DEF_MSG_SETTING_MIN] == 0)		// Default Message*/
				Write_line(0,&Serving_opt1_msg);
//			else
//			{
				/*Write_line(0,"SERVING         ");
				Write_lcd_comm(0xc0);
				for(i=0;i<16;i++)
				{
					Write_lcd_data(Message_arr[Parameter_val[DEF_MSG_SETTING_MIN]][i]);
				}*/
//				Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN]][0]);
				
//			}
			Dispensing = 1;		// Busy Serving
			Delay_time = Parameter_val[DEF_DELAY_TIME];
			Set_time_out();
		}		
	}
}


//******************** ACT KEY2 ****************************
// Program comes here when key2 is pressed.
// In setting mode this key will act as UP key
// Increments the value of Parameter_val and displays on LCD. 
void Act_key2(void)
{
	unsigned char i;
	if(Setting_mode)
	{
		if(Parameter!=0xff)
		{
			if((Parameter)==DEF_HALF_MODE)
			{
				Set_half_opt();
			}
			else if((Parameter)==DEF_BREW_SETTING)
			{
				Set_brew_mode();
			}
/*			else if(((Parameter-1)>=DEF_MSG_SETTING_MIN) && ((Parameter-1)<=DEF_MSG_SETTING_MAX)) 	// for message setting mode
	 		{
				if(Msg_arr_index==MAX_MSG-1)
					Msg_arr_index = MIN_MSG-1;
				else
					Msg_arr_index++;
	
				Parameter_val[Parameter-1]=Msg_arr_index;
			}
*/			else
			{
				Parameter_val[Parameter]++;	
				if((Parameter)==DEF_FLUSH_TOUT)
				{
					if(Parameter_val[Parameter]>MAX_TIME_LIMIT)
						Parameter_val[Parameter] = 0 ;
				}
			
				// IF added for max time limit 
				if((Parameter)!=DEF_REFIL_TIME)
				{
					if(Parameter_val[Parameter]>MAX_TIME_LIMIT)
						Parameter_val[Parameter] = 0;
				}
			}
			Disp_para_val();	
		}
	}
	else if(Counter_mode)
	{
		if(Counter==MAX_CNTR)
			Counter=1;
		else
			Counter++;
		
		// READ COUNTER FROM MEMORY
		Read_counter_val();
		// Display Counter name on LCD
		Disp_option();
		Display_counter_val();
	}
	
	else
	{
		if(Flush_reqd)
		{
//			Write_line(0,&Flush_msg);
			return;
		}
		else if(Dispensing==0)
		{	
			Counter = OPTION2;			/* Start UPDATING Counter */
			Update_counter_val();		/* END UPDATING Counter  */
//			if(Parameter_val[DEF_MSG_SETTING_MIN+1] == 0)		// Default Message*/
				Write_line(0,&Serving_opt2_msg);
//			else
//			{
/*				Write_line(0,"SERVING         ");
				Write_lcd_comm(0xc0);
				for(i=0;i<16;i++)
				{
					Write_lcd_data(Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+1]][i]);
				}
*/
//				Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+1]][0]);
				
//			}
			Dispensing=1;
			Set_time_out();
			if((Parameter_val[DEF_BREW_SETTING]!=1) || ((Parameter_val[DEF_BREW_SETTING]==1)&&(Parameter_val[DEF_BREW_TIME]==0)))
			// If Brewing Mode is off OR
			// If Brewing Mode is ON and Brew time is set to 0.0
				Delay_time = Parameter_val[DEF_DELAY_TIME];		
			else
			{
				Brew_time = Parameter_val[DEF_BREW_TIME];		// Start Brewing
				Brewing = 1; 
				Pre_brew_time = (WATER_PERC * Temp_water_timeout)/ 100;
				Water_timeout= Pre_brew_time;					// end brewing 				
			}
		}
	}
}

//******************** ACT KEY3 *******************************
// Program comes here when key3 is pressed.
// In setting mode this key will act as DN key
// Decrements the value of Parameter_val and displays on LCD. 
// If Parameter is 0 i.e. No action to be taken on UP and DN key
void Act_key3(void)
{
	unsigned char i;
	if(Setting_mode)
	{
		if (Parameter!=0xff)
		{
			if(Parameter==DEF_HALF_MODE)
				Set_half_opt();
			else if((Parameter)==DEF_BREW_SETTING)
				Set_brew_mode();
/*			else if(((Parameter - 1)>=DEF_MSG_SETTING_MIN) && ((Parameter-1)<=DEF_MSG_SETTING_MAX))
			{
				if(Msg_arr_index==MIN_MSG-1)
					Msg_arr_index=MAX_MSG-1;
				else
					Msg_arr_index--;
				
				Parameter_val[Parameter-1]=Msg_arr_index;
			}
*/		
			else
			{
				Parameter_val[Parameter]--;
				if((Parameter)==DEF_FLUSH_TOUT)
				{
					if(Parameter_val[Parameter]>MAX_TIME_LIMIT)
						Parameter_val[Parameter] = MAX_TIME_LIMIT ;
				}
				// IF added for max time limit 
				if((Parameter)!=DEF_REFIL_TIME)
				{
					if(Parameter_val[Parameter]>MAX_TIME_LIMIT)
						Parameter_val[Parameter] = MAX_TIME_LIMIT;
				}			}				
			Disp_para_val();	
		}
		
	}

	else if(Counter_mode)
	// Counter not to be reset 
		return;
	
	/*
	// if key3 is pressed in counter display mode the reset corresponding counter and display 
	// its value.
		if(Counter!=0)
		{
			unsigned char i;
			for(i=0;i<COUNTER_LENGTH;i++)
			{
				Counter_val[i]=0;
				Write_iic_byte((((Counter-1)*4) + i + DEF_OPTION_START_CNTR),Counter_val[i]);
			}
			Display_counter_val();
		}		
	
	*/
	
	else
	{
		if(Flush_reqd)
		{
//			Write_line(0,&Flush_msg);
			return;
		}
		else if (Dispensing == 0)
		{
			if(Parameter_val[DEF_HALF_MODE]==1)
			{
				Half_cup^=1;
				if(Half_cup)
					Write_line(0,"HALF CUP        ");
				else
					Write_line(0,"FULL CUP        ");
			}
			else
			{
				/* Start UPDATING Counter */
				Counter = OPTION3;
				Update_counter_val();			
				/* END UPDATING Counter  */
//				if(Parameter_val[DEF_MSG_SETTING_MIN+2] == 0)		// Default Message*/
					Write_line(0,&Serving_opt3_msg);
//				else
//				{
/*					Write_line(0,"SERVING         ");
	
					Write_lcd_comm(0xc0);
					for(i=0;i<16;i++)
					{
						Write_lcd_data(Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+2]][i]);
					}*/
//					Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+2]][0]);
//				}
				Dispensing=1;
				Delay_time = Parameter_val[DEF_DELAY_TIME];
				Set_time_out();
			}
		}
	}
}

//******************** ACT KEY4 *******************************
// Program comes here when key4 is pressed.
// In setting mode this key will act as MODE key
// Write previous value to eeprom. NOTE - if parameter=0 then do not read from eeprom
// Display corresponding parameter. - Disp.c
// Display parameters value.
// If Parameter is 0 i.e. No action to be taken on UP and DN key
void Act_key4(void)
{
	unsigned char i,j;
	if(Counter_mode)
		return;

	else if(Setting_mode)
	{
		if(Parameter!=0xff)
			// Sets the Parameter value 
			Write_iic_byte((DEF_PARA_LOC+Parameter),Parameter_val[Parameter]);
		
		if(Parameter==MAX_PARA)
			Parameter=MIN_PARA;	
		else
		{
			Parameter++;
			
			if((Parameter_val[DEF_HALF_MODE]==1) && (Parameter==DEF_OPTION3))
				Parameter += OPTION_LENGTH;			
			
			if(Parameter_val[DEF_BREW_SETTING]==0 && (Parameter ==DEF_BREW_TIME))
			//If brew mode off then doesnot takes setting for brew time
				Parameter=MIN_PARA;
			
		}
		Disp_parameter();
		Parameter_val[Parameter] = Read_iic_byte(DEF_PARA_LOC+Parameter);
//		if(((Parameter-1)>=DEF_MSG_SETTING_MIN) && ((Parameter-1)<=DEF_MSG_SETTING_MAX))
//			Msg_arr_index = Parameter_val[Parameter-1];
//		else
			Disp_para_val();
	}
	
	else
	{
		if(Flush_reqd)
		{
			Counter=0;
			Write_line(0,&Flushing_msg);
			Dispensing = 1;
			Set_time_out();
			Flush_reqd = 0;
			Flushing =1;
		}
		else if(Dispensing==0)
		{
			Hot_water_reqd = 1;
			/* Start UPDATING Counter */
			Counter = OPTION4;
			Update_counter_val();			
			/* END UPDATING Counter  */
			Write_line(0,&Serving_opt4_msg);
			Dispensing=1;
			Set_time_out();
		}
	}
}



// *************************** CHECK FOR POWER ON KEY **********************************
// Functions calls only at power on.
// If KEY4 press then unit goes to setting mode.
void Chk4poweron_key(void)
{
	unsigned char j;
	Dbnc_key_code = Kbd_pos = 0xff;
	for(j=0;j<20;j++)
	{
		Sense_kbd();
		if(New_key_found)
		{
			New_key_found=0;
			if(Kbd_pos==KEY4)
			{
				Setting_mode=1;
				Write_line(0,&Setting_msg);
				break;
			}
			else if(Kbd_pos==KEY1)
			{
				Counter_mode=1 ;
				Write_line(0,&Counter_msg);
				break; 
			}
		}
	}
}


// Initializes port
void Initports(void)
{
	DDRA = Def_ddra;
	PTA  = Def_porta;
	DDRB = Def_ddrb;
	PTB  = Def_portb;
	DDRD = Def_ddrd;
	PTD	 = Def_portd;
}


//************************* SWITCH ON RELAY *****************************
// For switching on RELAYS as per selection made
void Switch_on_relay(void)
{	
	
	if(Delay_time==0)
	{
		/*if ((Premix1_timeout > 0 ) && (Rly_premix1==0))
			Rly_premix1=1;		
	
		else if ((Premix2_timeout > 0 ) && (Rly_premix2==0))
			Rly_premix2=1;		

		else if ((Premix3_timeout > 0 ) && (Rly_premix3==0))
			Rly_premix3=1;		
		*/
		
		if((Premix_timeout>0) && ((Rly_premix1==0)||(Rly_premix2==0)||(Rly_premix3==0)))
		{
			switch(Counter)
			{
				case 1:
					{
						Rly_premix1=1;
						break;
					}
				case 2:
					{
						Rly_premix2=1;
						break;
					}
				case 3:
					{
						Rly_premix3 = 1;
						break;
					}
			}
		}	
	}

	if(((Counter>=1) && (Counter<=3)) && (Water_timeout>0) && ((Rly_water1==0)||(Rly_water2==0)||(Rly_water3==0)))
	{
		switch(Counter)
		{
			case 1:
				{
/*					Write_line(0,"WATER1 TO       ");
					Write_lcd_comm(0xc2);
					Write_lcd_data(Water_timeout);
*/					Rly_water1=1;
					break;
				}
			case 2:
				{
/*					Write_line(0,"WATER2 TO       ");
					Write_lcd_comm(0xc2);
					Write_lcd_data(Water_timeout);
*/					Rly_water2=1;
					break;
				}
			case 3:
				{
/*					Write_line(0,"WATER3 TO       ");
					Write_lcd_comm(0xc2);
					Write_lcd_data(Water_timeout);
*/					Rly_water3=1;
					break;
				}
		}
	}
	
	else
	{
		if ((Water1_timeout > 0 ) && (Rly_water1==0))
			Rly_water1=1;		
	
		else if ((Water2_timeout > 0 ) && (Rly_water2==0))
			Rly_water2=1;		

		else if ((Water3_timeout > 0 ) && (Rly_water3==0))
			Rly_water3=1;		
	}
	

	if ((Refil_timeout>0) && (Rly_refil==0) && (Brewing==0))
		Rly_refil=1;		
	

}
// ************************** SWITCH OFF RELAY *******************************************
// For switching on RELAYS as per selection made
void Switch_off_relay(void)
{	

/*	if (Premix1_timeout==0 )
		Rly_premix1=0;		
	
	if (Premix2_timeout==0 )
		Rly_premix2=0;		

	if (Premix3_timeout==0 )
		Rly_premix3=0;		
*/

	if((Counter>=1) && (Counter<=3))		// Between option 1 to Option 3
	{
		if(Premix_timeout==0)
		{
			switch(Counter)
			{
				case 1:
					{
						Rly_premix1=0;
						break;
					}
				case 2:
					{
						Rly_premix2=0;
						break;
					}
				case 3:
					{
						Rly_premix3=0;
						break;
					}
			}		
		}
		if(Water_timeout==0)
		{
			switch(Counter)
			{
				case 1:
					{
						Rly_water1=0;
						break;
					}
				case 2:
					{
						Rly_water2=0;
						break;
					}
				case 3:
					{
						Rly_water3=0;
						break;
					}
			}
		}
	}		
	else
	{
		if (Water1_timeout==0 )
			Rly_water1=0;		
		if (Water2_timeout==0 )
			Rly_water2=0;		
		if (Water3_timeout==0 )
			Rly_water3=0;		
	}
	if (Refil_timeout==0 )
	{
		Rly_refil=0;		
		Set_flush_timeout();
	}
	
}

//*********************** READ PARAMETER VALUES **********************************************
// Reads parameter values for all premixes and water levels from memory
void Read_all_para(void)
{
	unsigned char i;
	for(i=MIN_PARA;i<=MAX_PARA;i++)
	{
		Parameter_val[i] = Read_iic_byte(DEF_PARA_LOC + i);
	}
}

/******************************SET TIME OUT***********************************/
// Set time outs for a particular mode chosen by user

void Set_time_out(void)
{
	if((Hot_water_reqd) && (!Flush_reqd))
	{
		if(Half_cup)  // Sets Half Time for Half cup on Hot Water
		{
			Water1_timeout  = Parameter_val[DEF_HOT_WATER ]/2;
			Water2_timeout  = Parameter_val[DEF_HOT_WATER + 1]/2;
			Water3_timeout  = Parameter_val[DEF_HOT_WATER + 2]/2;
		}
		else
		{
			Water1_timeout  = Parameter_val[DEF_HOT_WATER];
			Water2_timeout  = Parameter_val[DEF_HOT_WATER + 1];
			Water3_timeout  = Parameter_val[DEF_HOT_WATER + 2];
		}
		
	}
	
	else if(Flush_reqd)
	{
		Water1_timeout = Parameter_val[DEF_FLUSH_TIME];
		Water2_timeout = Parameter_val[DEF_FLUSH_TIME];
		Water3_timeout = Parameter_val[DEF_FLUSH_TIME];
	}
	
	else if((Parameter_val[DEF_HALF_MODE] == 1) && (Half_cup))
	{

		if((Parameter_val[DEF_BREW_SETTING] == 1) && (Parameter_val[DEF_BREW_TIME]!=0) && (Counter == OPTION2))
		{
			Temp_premix_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 1]/2; 
			Temp_water_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2]/2;
			Brewing =1; 
		}
		else
		{
			Premix_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 1]/2; 
	//		Premix2_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2]/2;
	//		Premix3_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 3]/2;
			Water_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2]/2;
	//		Water2_timeout  = Parame	ter_val[(Kbd_pos * OPTION_LENGTH) + 5]/2;
	//		Water3_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 6]/2;
			Brewing = 0;
		}
	}
	else
	{
		if((Parameter_val[DEF_BREW_SETTING] == 1) && (Parameter_val[DEF_BREW_TIME]!=0) && (Counter == OPTION2))
		{
			Temp_premix_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 1]; 
			Temp_water_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2];
			Brewing =1; 
		}
		else
		{
			Premix_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 1]; 
	//		Premix2_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2];
	//		Premix3_timeout = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 3];
			Water_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 2];
	//		Water2_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 5];
	//		Water3_timeout  = Parameter_val[(Kbd_pos * OPTION_LENGTH) + 6];
			Brewing = 0;
		}
	}
}


/* set refill relay on after all thet timeouts are over 
and machine is still in dispensing mode */
void Check_n_set_refil_relay()
{
//	if((Premix1_timeout == 0) && (Premix2_timeout == 0) && (Premix3_timeout ==0) && (Water1_timeout ==0) && (Water2_timeout ==0) && (Water3_timeout ==0) && (Dispensing) && (Refil_timeout==0))
	if((Premix_timeout == 0) && (Water_timeout==0) && (Water1_timeout ==0) && (Water2_timeout ==0) && (Water3_timeout ==0) && (Dispensing) && (Refil_timeout==0))
	{
		if(Brewing==0)
		{
			Refil_timeout = Parameter_val[DEF_REFIL_TIME];
			Switch_on_relay();
		}
		else
		{
			Refil_timeout = 0 ;
		}
	}
}


// display all options to see there counter values
// Display Counter on LCD 
void Display_counter_val(void)
{	
	unsigned char i;
	Write_lcd_comm(COUNTER_ADD);		// initialises the starting add for counter
	for(i=4;i>0;i--)
	{	
		Write_lcd_data(0x30 + ((Counter_val[i-1]&0xf0)>>4));	// disp upper nibble first	
		Write_lcd_data(0x30 + (Counter_val[i-1]&0x0f));	// disp lower nibble second	
	}	
}


// This function is called in counter mode. When Key2 is press to see all options for half 
// or full cup
/* Read 8 byte Counter value from MEMORY */
void Read_counter_val(void)
{
	unsigned char i;
	for(i=0;i<COUNTER_LENGTH;i++)
	{
		Counter_val[i] = Read_iic_byte(((Counter-1) * 4) + i + DEF_OPTION_START_CNTR);
		if(Counter_val[i] > 0x99)
		{
			Counter_val[i]=0;
			Write_iic_byte((((Counter-1)*4) + i + DEF_OPTION_START_CNTR),Counter_val[i]);
		}
	}	
}

void Update_counter_val(void)
{
	unsigned char i;
	Read_counter_val();			// Reads the Counter Value from Memory and sets to
								// Counter_val array	
	for(i=0;i<COUNTER_LENGTH;)
	{
		if((Counter_val[i] & 0x0f) == 0x9)
		{
			Counter_val[i] &= 0xf0;				// reset lower nibble
			if((Counter_val[i] & 0xf0) == 0x90)	// if upper nibble == 9 then	
			{
				Counter_val[i] &= 0x0f;			// reset upper nibble	
				i++;							// and increase lower nibble of next counter_val
			}
			else
			{				
				Counter_val[i] += 0x10;		 	// increase upper nibble				
				break;
			}
		}			
		else
		{
			Counter_val[i] += 0x1;
			break;		
		}		

/*			
		if(Counter_val[i] == 9)
		{
			Counter_val[i] = 0;
			i++;
		}
		else
		{
			Counter_val[i]++;
			break;
		}	
*/
	}

	Write_counter_val();
}



void Write_counter_val(void)
{
	unsigned char i;
	for(i=0;i<COUNTER_LENGTH;i++)
	{
		Write_iic_byte((((Counter-1)*4) + i + DEF_OPTION_START_CNTR),Counter_val[i]);
	}
}


// Read Flush Time out from PARAMETER_VAL

void Set_flush_timeout(void)
{
	Flush_timeout = Parameter_val[DEF_FLUSH_TOUT];
	One_min_over = 0;
	Secs =0;
}


void Set_half_opt(void)
{
	if(Parameter_val[DEF_HALF_MODE] == 1)
		Parameter_val[DEF_HALF_MODE]=0;
	else
		Parameter_val[DEF_HALF_MODE]=1;
}



void Set_brew_mode(void)
{
	if(Parameter_val[DEF_BREW_SETTING] == 1)
		Parameter_val[DEF_BREW_SETTING]=0;
	else
		Parameter_val[DEF_BREW_SETTING]=1;
}

//************************************PREVIOUS STATE********************************
// If a warm boot is there then previous state is restored 

void Previous_state(void)
{
	if(Mem_bad)
 	{
 		Write_line(0,&Memory_msg);
		while(1);
	}
	
	else if(Setting_mode)			// In setting mode 
	{
		Disp_parameter();
		Disp_para_val();
	}
	else if(Counter_mode)		// In counter mode 
	{
		if(Counter)
		{
			Read_counter_val();					// Reads Counter from memory
			Disp_option();						// Display Counter name on LCD
			Display_counter_val();				// Displays Counter value
		}
		else
			Write_line(0,&Counter_msg);
	}

	else if((Flush_reqd) || (Flushing))		// If Flush is Required or Machine is Flushing
	{
		if(Flushing)		// If flushing
		{
			Write_line(0,&Flushing_msg);
			Switch_on_relay();
		}
		else
		{
			Write_line(0,&Flush_msg);
		}
	}

	else if(Dispensing)		 // for dispensing 
	{
		switch(Counter)		
		{
			
			case 1:
				{
//					if(Parameter_val[DEF_MSG_SETTING_MIN] == 0)		// Default Message
						Write_line(0,&Serving_opt1_msg);
//					else											// If the user has set the message  in setting mode for this key
//						Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN]][0]);				
					break;
				}
				
			case 2:
				{
//					if(Parameter_val[DEF_MSG_SETTING_MIN+1] == 0)		// Default Message
						Write_line(0,&Serving_opt2_msg);



//					else											// If the user has set the message  in setting mode for this key
//						Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+1]][0]);				
					break;
				}
					
			case 3:
				{
//					if(Parameter_val[DEF_MSG_SETTING_MIN+2] == 0)		// Default Message
						Write_line(0,&Serving_opt3_msg);
//					else											// If the user has set the message  in setting mode for this key
//						Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+2]][0]);				
					break;
				}
								
			case 4:
				{
//					if(Parameter_val[DEF_MSG_SETTING_MIN+3] == 0)		// Default Message
						Write_line(0,&Serving_opt4_msg);
//					else											// If the user has set the message  in setting mode for this key
//						Write_line(0,&Message_arr[Parameter_val[DEF_MSG_SETTING_MIN+3]][0]);				
					break;
				}
	
		}
		Switch_on_relay();
	}
	

	else if(Parameter_val[DEF_HALF_MODE] == 1)		// Half Cup Mode 
	{
		if(Half_cup)
			Write_line(0,"HALF CUP        ");
		else
			Write_line(0,"FULL CUP        ");
	}
	else		// Ready 
	{
		Disp_cup_status();
	}
}