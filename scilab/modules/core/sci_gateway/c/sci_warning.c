/*--------------------------------------------------------------------------*/ 
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
#include "Scierror.h"
#include "warningmode.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_warning)(char *fname,unsigned long fname_len)
{
	static int l1 = 0,n1 = 0,m1 = 0;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_strings )
	{
		char **Input_Strings = NULL;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_Strings);

		if ( (m1*n1) == 1 )
		{
			if ( (strcmp(Input_Strings[0],"off") == 0) || (strcmp(Input_Strings[0],"on") == 0) )
			{
				if (strcmp(Input_Strings[0],"off") == 0)
				{
					setWarningMode(FALSE);
				}
				else  /* on */
				{
					setWarningMode(TRUE);
				}
			}
			else
			{
				if (strcmp(Input_Strings[0],"query") == 0)
				{
					char *Output=NULL;

					Output=(char*)MALLOC(4*sizeof(char));

					if (getWarningMode())
					{
						strcpy(Output,"on");
					}
					else
					{
						strcpy(Output,"off");
					}

					n1=1;
					CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
					LhsVar(1) = Rhs+1;
					C2F(putlhsvar)();
					if (Output) {FREE(Output);Output=NULL;}
					freeArrayOfString(Input_Strings,n1);
					return 0;
				}
				else if ( getWarningMode() )
				{
					sciprint(_("WARNING: %s"),Input_Strings[0]);
					sciprint("\n");
					freeArrayOfString(Input_Strings,n1);
				}
			}
		}
		else
		{
			int i = 0;
			if ( getWarningMode() )
			{
				for (i = 0;i < m1*n1;i++)
				{
					sciprint(_("WARNING: %s\n"),Input_Strings[i]);
				}
				sciprint("\n");
			}
			freeArrayOfString(Input_Strings,m1*n1);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
		return 0;
	}
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
