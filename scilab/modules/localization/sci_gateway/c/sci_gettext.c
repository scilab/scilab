/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_gettext)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
		{
			static int l1,n1,m1;

			char *msgid=NULL;
			char *TranslatedString=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			msgid=cstk(l1);

			/* We always have something from this functions because gettext
			 * is returning the same string if it cannot find it */
			TranslatedString=gettext(msgid);

			n1=1;
			CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString), &m1),&n1,&TranslatedString);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
			return 0;
		}
	else
		{
			Scierror(999,"%s: Wrong type for first input argument: String expected.\n",fname);
			return 0;
		}
}
/*--------------------------------------------------------------------------*/
