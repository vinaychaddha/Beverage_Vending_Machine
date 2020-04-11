
#include "extern_var.c"
#include "extern_port.c"

#define MAX_KEYS 4
#define MAX_REPEAT_KEY_CNTR 100
#define MID_REPEAT_KEY_CNTR	70
#define DEF_DBNC_CNTR 5


#pragma CONST_SEG ROM
								
const unsigned char Scan_table[4]={0x0E, 0x0D, 0x0B, 0x07};

const unsigned char Kbd_table[4]= {
									0xEE,
									0xDE,
									0xBE,
									0x7E 
    								};
                                           

/*
If a key has been detected, check the following:
	- Is it a totally new code
	- Is it a key that is being debounced currently
	- Is it a key which has been accepted i.e. repeat key
	
	If it is totally new key code
		Set debounce counter to default value
		Save the key_code in dbnc_key_code variable
		Return 0
	
	If the key code is equal to dbnc_key_code
		Decrement dbnc_cntr
		If dbnc_cntr == 0
			Set repeat_key_cntr to max_repeat_key_cntr
			Save the key number in kbd_pos variable
			Return 1
			
	If key code is equal to kbd_pos i.e. repeat key
		Decrement repeat_key_cntr
		If repeat_key_cntr == 0
			Set repeat_key_cntr to mid_repeat_key_cntr
			Return 1
Else
	Return 0
*/


void Sense_kbd(void)
{

	Lcd_data = Scan_table[Key_scan_cntr];
	Key_code = Lcd_data << 4;
	Key_code &= 0xf0;
	if(Kbd==0)
		Key_code |= 0x0e;
	/*
	The current status of Keys is in Key_code variable.
	Valid Key_codes are defined in array Kbd_table
	Compare Key_code variable with all the entries in Kbd_table
	*/

		
	i = 0x00;
	while ((i < sizeof(Kbd_table)) && (Key_code != Kbd_table[i]))
		i++;
	
	/*
	Check if the Key is equal to the previous key detected
	
	*/

	if (i == Kbd_pos)						/* Check Repeat Key*/
	{
		if(Kbd_pos == MAX_KEYS)
			goto Change_sense;

		else if((Setting_mode)&&((Kbd_pos==KEY2) || (Kbd_pos==KEY3)))
		{	
			Repeat_key_cntr--;
			if(Repeat_key_cntr == 0)
			{
				Repeat_key_cntr = MID_REPEAT_KEY_CNTR;
				New_key_found = 1;
			}
		}
	}

	
	else if (i == Dbnc_key_code)
	{
		Dbnc_cntr--;
		if (Dbnc_cntr == 0)
		{
			Kbd_pos =  i;

			if(Kbd_pos == MAX_KEYS)
				goto Change_sense;
			else
				New_key_found = 1;
		}
	}
		
	else
	{
		Dbnc_key_code = i;
		Dbnc_cntr = DEF_DBNC_CNTR;
		Repeat_key_cntr = MAX_REPEAT_KEY_CNTR;
	}

	return;
	

Change_sense:
	if(Key_scan_cntr < 3)
		Key_scan_cntr++;
	else
		Key_scan_cntr = 0;
		
	Lcd_data = Scan_table[Key_scan_cntr];
}

