/*--------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                     */
/* Author: Vincent COUVERT                                                  */
/* desc: interface for iswaitingforinput primitive                          */
/*--------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "ConsoleIsWaitingForInput.h"
#include "scilabmode.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_iswaitingforinput(char *fname,unsigned long fname_len)
{
	BOOL res = FALSE;
	int un = 1, outIndex = 0;

	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

        if (getScilabMode() == SCILAB_STD) {
          res = ConsoleIsWaitingForInput();
        } else {
		sciprint(_("Not implemented in this mode.\n"));
	}

        CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,&un,&un,&outIndex);
        *istk(outIndex) = res;
	
        LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
