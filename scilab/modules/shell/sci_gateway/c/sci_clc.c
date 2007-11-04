/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                              */
/* Author : Allan Cornet                                                             */
/* desc : interface for clc primitive                                                */
/*-----------------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "stack-c.h"
#include "clc.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_clc) _PARAMS((char *fname,unsigned long fname_len))
{
	BOOL res = FALSE;
	int nblines = -1;

	Rhs = Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		if ( GetType(1) == sci_matrix )
		{
			int l1,m1=1,n1=1;
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
			nblines=*istk(l1);

			if (nblines < 0)
			{
				Scierror(999,_("Invalid parameter.\n"));
				return 0;
			}
		}
		else
		{
			Scierror(999,_("Invalid parameter.\n"));
			return 0;
		}
	}
	else
	{
		/* Clear console full */
		nblines = -1;
	}

	res = clc(nblines);
	if (!res)
	{
		sciprint(_("not implemented in this mode.\n"));
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
