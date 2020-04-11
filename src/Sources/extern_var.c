
#pragma DATA_SEG __SHORT_SEG MY_ZEROPAGE
//#pragma DATA_SEG __SHORT_SEG _DATA_ZEROPAGE

extern struct
{
	unsigned int One_tick_over:1;
	unsigned int Half_sec_over:1;
	unsigned int One_sec_over:1;		
	unsigned int New_key_found:1;
	unsigned int Setting_mode:1;
	unsigned int Iic_bad:1;
	unsigned int Mem_bad:1;
	unsigned int Ms100_over:1;
	unsigned int Dispensing:1;
	unsigned int Counter_mode:1;	
	unsigned int Hot_water_reqd:1;
	unsigned int Flush_reqd:1;	
	unsigned int One_min_over:1;
	unsigned int Half_cup:1;
	unsigned int Brewing:1;
	unsigned int Flushing:1;
}sStatus;


#define Dispensing		sStatus.Dispensing
#define Mem_bad			sStatus.Mem_bad
#define Iic_bad			sStatus.Iic_bad
#define Setting_mode 	sStatus.Setting_mode
#define One_tick_over 	sStatus.One_tick_over
#define Half_sec_over	sStatus.Half_sec_over
#define One_sec_over 	sStatus.One_sec_over
#define New_key_found	sStatus.New_key_found
#define Ms100_over      sStatus.Ms100_over
#define Counter_mode	sStatus.Counter_mode
#define Hot_water_reqd	sStatus.Hot_water_reqd
#define Flush_reqd		sStatus.Flush_reqd
#define One_min_over	sStatus.One_min_over
#define Half_cup		sStatus.Half_cup
#define Brewing 		sStatus.Brewing
#define Flushing		sStatus.Flushing

#define MAX_OPTION 4			// Maximum options like op1,op2,op3,op4


#define MIN_PARA	0
#define MAX_PARA	15	
// These def values are for parameter 
#define DEF_HALF_MODE 	0
#define DEF_OPTION3 	5
#define DEF_HOT_WATER 	7
#define DEF_REFIL_TIME 	10
#define DEF_FLUSH_TIME 	11
#define DEF_FLUSH_TOUT 	12
#define DEF_DELAY_TIME 	13
#define DEF_BREW_SETTING 14
#define DEF_BREW_TIME 15

//#define PRE_BREW_TIME 10     // Water is switched for 1 second 

#define MAX_TIME_LIMIT 99

extern unsigned char Secs,Pre_brew_time;
extern unsigned char Kbd_pos,Max_iic_bytes,Repeat_key_cntr,Parameter,Dbnc_key_code;
extern unsigned char Parameter_val[],Ascii_unit,Ascii_ten,Ascii_hun;


//extern unsigned char Premix1_timeout,Premix2_timeout,Premix3_timeout;
extern unsigned char Premix_timeout;
extern unsigned char Water_timeout;
extern unsigned char Water1_timeout,Water2_timeout,Water3_timeout,Rly_on_dly_tout;
extern unsigned char Refil_timeout;
extern unsigned char Flush_timeout;
extern unsigned char Delay_time;
extern unsigned char Brew_time;
// byte for counter
extern unsigned char Counter_val[],Counter;


#define Tic_in_hsec 100		// Constant for half second interval
#define Tic_in_1sec	200		// Constant for one second interval


#define KEY1	0x00			
#define KEY2	0x01			
#define KEY3	0x02			
#define KEY4	0x03			

// for memory location
#define DEF_PARA_LOC	0x0

// for lcd location for printing values for premixes\ water\ timeouts
#define PARA_VAL_ADD	0xc3
#define COUNTER_ADD		0xc0


#define DEF_OPTION_START_CNTR 40	// FOR MEMORY LOCATION OF COUNTERS IN MEMORY
#define COUNTER_LENGTH	4			// This indicates 8 bcd bytes for counter.
#define MAX_CNTR	4				// maximum option
#define OPTION1 1
#define OPTION2 2
#define OPTION3 3
#define OPTION4 4

#define RLY_ON_DLY_TICKS	10

#define OPTION_LENGTH 2				

/*
// for user defined messages
#define MAX_MSG 10
#define MIN_MSG 1
#define DEF_MSG_SETTING_MIN 26
#define DEF_MSG_SETTING_MAX 28

//#pragma DATA_SEG DEFAULT_RAM

extern unsigned char Msg_arr_index;		// Ranges from MIN_MSG to MAX_MSG-1
*/

// FROM IIC
extern unsigned char Temp, Tmp_cntr_iic,Iic_counter,Iic_buff[4]; 

// from INITLCD
extern unsigned char Lcd_cntr,Lcd_pointer;


// from sensekbd

extern unsigned char Key_scan_cntr,Key_code,Dbnc_cntr,i;
extern unsigned char Dbnc_key_code;


// from timer
extern unsigned char Ticks, Small_ticks;


extern unsigned int Test_pattern;

#define WATER_PERC 30