/*--------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "errmds.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/ 
int C2F(errmds)(integer *num, integer *imess, integer *imode)
{
	/* 100000 is a constant used in fortran to code errct value :( */
	#define ERRCATCH_CONST_ERRCT 100000
	*num = 0;
	if (C2F(errgst).errct > 0) 
	{
		*num = C2F(errgst).errct - ERRCATCH_CONST_ERRCT * (int)( C2F(errgst).errct / ERRCATCH_CONST_ERRCT );
		*imode = C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
	} 
	else if (C2F(errgst).errct < 0) 
	{
		*num = -1;
		*imode = -C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
	}
	*imess = *imode / 8;
	*imode = *imode - 8* (*imess);
	return 0;
} 
/*--------------------------------------------------------------------------*/ 
