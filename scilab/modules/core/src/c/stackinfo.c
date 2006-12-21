/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef HAVE_LIMITS_H
#include <limits.h>
#define MAXLONG LONG_MAX
#else 
#ifdef HAVE_VALUES_H
#include <values.h>
#endif /* HAVE_VALUES_H */
#endif /* !HAVE_LIMITS_H */

#include "stackinfo.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/
#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif

#ifndef LONG_MAX 
#define LONG_MAX 2147483647L 
#endif 

#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif
/*-----------------------------------------------------------------------------------*/
integer C2F(getstackinfo)(integer *total,integer *used)
{
	*used = C2F(vstk).lstk[C2F(vstk).isiz - 1] - C2F(vstk).lstk[C2F(vstk).bot - 1] + 1;
	*total = C2F(vstk).lstk[C2F(vstk).isiz - 1] - C2F(vstk).lstk[0];
	return(0);
}
/*-----------------------------------------------------------------------------------*/
integer C2F(getgstackinfo)(integer *total,integer *used)
{
	*used = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1] + 1;
	*total = C2F(vstk).lstk[C2F(vstk).gbot - 1] - C2F(vstk).lstk[C2F(vstk).isiz + 1] ;
	return(0);
}
/*-----------------------------------------------------------------------------------*/
integer C2F(getvariablesinfo)(integer *total,integer *used)
{
	*used = C2F(vstk).isiz - C2F(vstk).bot - 1 ;
	*total = C2F(vstk).isiz - 1;
	return 0;
}
/*-----------------------------------------------------------------------------------*/
integer C2F(getgvariablesinfo)(integer *total,integer *used)
{
	*used = C2F(vstk).gtop - C2F(vstk).isiz - 1;
	*total = 10000 - C2F(vstk).isiz - 1;
	return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL is_a_valid_size_for_scilab_stack(int sizestack)
{
	BOOL bOK=TRUE;
	double dsize = ((double) sizeof(double)) * (sizestack);
	unsigned long ulsize = ((unsigned long)sizeof(double)) * (sizestack);
	if ( dsize != (double) ulsize)
	{
		bOK=FALSE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
unsigned long get_max_memory_for_scilab_stack(void)
{
	return MAXLONG/sizeof(double);
}
/*-----------------------------------------------------------------------------------*/

