/*--------------------------------------------------------------------------*/
#include "returnanan.h"
/*--------------------------------------------------------------------------*/
double C2F(returnanan)(void)
{
	static int first = 1;
	static double nan = 1.0;

	if ( first )
	{
		nan = (nan - (double) first)/(nan - (double) first);
		first = 0;
	}
	return (nan);
}
/*--------------------------------------------------------------------------*/
