/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_oemtochar(char *fname,unsigned long l)
{
	int l1 = 0,n1 = 0,m1 = 0;
	char *Output = NULL;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)	
	{
		char *OEMstring = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		OEMstring = cstk(l1);
		
		Output=(char*)MALLOC((strlen(OEMstring)+1)*sizeof(char));
		if (getScilabMode() == SCILAB_STD)
		{
			OemToChar(OEMstring,Output);
		}
		else
		{
			wsprintf(Output,"%s",OEMstring);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
		return 0;
	}
	
	n1 = 1;
	CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
