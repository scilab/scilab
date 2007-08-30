/*------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                   */
/* Authors : Allan Cornet						                          */
/* desc : interface for tohome primitive                                  */
/*------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "machine.h"
#include "stack-c.h"
#include "tohome.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_tohome) _PARAMS((char *fname,unsigned long fname_len))
{
	BOOL bOK = FALSE;

	CheckRhs(0,0);
	CheckLhs(0,1);

	bOK = tohome();

	if (!bOK)
	{
		sciprint("not implemented in this mode.\n");
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0 ;
}
/*-----------------------------------------------------------------------------------*/
