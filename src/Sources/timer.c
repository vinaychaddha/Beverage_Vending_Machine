
#include "hidef.h"
#include "jl3.h"
#include "extern_var.c"	
#include "extern_port.c"

//#pragma DATA_SEG __SHORT_SEG MY_ZEROPAGE

volatile struct
{
  	unsigned int Prescaler_bits:3;
  	unsigned int Dummy:1;
  	unsigned int Rst_timcntr_psc:1;
  	unsigned int Stop_tim_cntr:1;
  	unsigned int Enable_ovf_int:1;
  	unsigned int Ovf_flag:1;
}sTsc @0x20;

/* Initialise the timer*/
void Init_timer(void)
{
	TMODH = 0x0;
	TMODL = 0xFA;								/*Timer overflow interrupt at 250us*/
	TSC = 0x40;
	EnableInterrupts;							/*Predefined macro in "hidef.h"*/
}	

/*-----------------------------------------------------------------------------------------
Routine		:_Timint (Timer overflow interrupt routine)
Inputs		:None
Outputs		:Updated value of Ticks variable
Notes		:Called upon every timer over flow interrupt (5000us at present)
-----------------------------------------------------------------------------------------*/
#pragma TRAP_PROC									
void _Timint(void)
{
	sTsc.Ovf_flag = 0;
	Small_ticks++;
	if(Small_ticks==20)
	{		
		One_tick_over=1;
		Small_ticks=0;
	}
}


#pragma TRAP_PROC									
void _Dummy(void)
{
	;
}


void Setime(void)
{
	Ticks++;	
	if((Ticks % 20) == 0)
		Ms100_over = 1;
	
	if(Ticks==Tic_in_hsec)
		Half_sec_over=1;
	if(Ticks==Tic_in_1sec)
	{
		Ticks=0;
		One_sec_over=1;
		Half_sec_over=1;

		Secs++;			// Increments seconds
		
		if(Secs==60)
		{
			Secs=0;
			One_min_over=1;
		}
	}
}

