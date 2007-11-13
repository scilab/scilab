/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "errmds.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(errmds)(integer *num, integer *imess, integer *imode)
{
	*num = 0;

	if (C2F(errgst).errct > 0) 
	{
		*num = C2F(errgst).errct - C2F(errgst).errct / isizt * isizt;
		*imode = C2F(errgst).errct / isizt;
	} else if (C2F(errgst).errct < 0) 
	{
		*num = -1;
		*imode = -C2F(errgst).errct / isizt;
	}
	*imess = *imode / 8;
	*imode -= *imess << 3;

	return 0;
} 
/*-----------------------------------------------------------------------------------*/ 
