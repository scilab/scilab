#include <stdio.h>

/*-----------------------------------------------------------------\
 Function Name	: int
 Returns Type	: unsigned
 	----Parameter List
	1. get_byte_value( unsigned char *start , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/
int get_1byte_value( char *start )
{
	return (int) *start;
}

/*-----------------------------------------------------------------\
 Function Name	: int
 Returns Type	: unsigned
 	----Parameter List
	1. get_ushort_value( unsigned char *start , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/
int get_2byte_value( char *start )
{
	int value = 0;

	value = (unsigned char)*start | (((unsigned char)*(start +1)) << 8);

	return value;
}

/*-----------------------------------------------------------------\
 Function Name	: int
 Returns Type	: unsigned
 	----Parameter List
	1. get_ulong_value( unsigned char *start , 
 	------------------
 Exit Codes	: 
 Side Effects	: 
--------------------------------------------------------------------
 Comments:
 
--------------------------------------------------------------------
 Changes:
 
\------------------------------------------------------------------*/
int get_4byte_value( char *start )
{
	int value = 0;

	value = ((unsigned char)*start)
		|(((unsigned char)*(start +1)) << 8) 
		|(((unsigned char)*(start +2)) << 16) 
		|(((unsigned char)*(start +3)) << 24);

	/*	printf("String=0x%x %x %x %x:%u = %d\n", *start, *(start +1), *(start +2), *(start +3), *(start +3), value);*/

	return value;
}


