

/*---------------------------------------------------------------------------------------------

 Date	:	22/11/01

 Name	:	Chk_dec_timeout

 Input	:	pointer to 1 byte value to be decremented.
 
 Variables	:  *dec_val

 Output	:	Return 1 if the timeout decrements to 00 else return 0.

 Description : 
 			a)	If the value is already 00, return 00.				
			b)	If the input value >0, decrement the value & return 1
				if the value has reached to 00 after decrementing .
			c)	If the value >0, even after decrementing , return 0.
			
e.g. let the input value is 5,go to case (B),
		check is it >0; y
		decrement the value , return 0.
		now the input value is 4, go to case (B).
		is 4>0;y
		decrement the value , return 0.
		a stage comes when the input value is 1,in this case go to 
		case (B).1>0;y
		decrement it , 1-1 =0; return 1
		
		
-----------------------------------------------------------------------------------------*/

unsigned char Chk_dec_timeout(unsigned char *time_out)
{
	if(*time_out==0)
		return(0);
	else
	{
		*time_out=*time_out-1;

 		if(*time_out==0)
	  		return(1);
 		else
	  		return(0);
	}
}

