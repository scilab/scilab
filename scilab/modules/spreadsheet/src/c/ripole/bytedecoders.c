#include <stdio.h>
#include "bytedecoders.h"

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
int get_1byte_value( unsigned char *start )
{
    return (int) * start;
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
int get_2byte_value( unsigned char *start )
{
    int value = 0;

    value = *start | ((*(start + 1)) << 8);

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
int get_4byte_value(unsigned char *start )
{
    int value = 0;

    value = (*start)
            | ((*(start + 1)) << 8)
            | ((*(start + 2)) << 16)
            | ((*(start + 3)) << 24);

    /*	printf("String=0x%x %x %x %x:%u = %d\n", *start, *(start +1), *(start +2), *(start +3), *(start +3), value);*/

    return value;
}


