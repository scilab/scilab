/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007/2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_localization.h"
#include "setgetlanguage.h"
#include "Scierror.h"
#include "sciprint.h"
#include "warningmode.h"
#include "defaultlanguage.h"
#include "MALLOC.h"
#include "loadhashtableslocalization.h"
#include "localization.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_setlanguage(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;

	CheckRhs(0,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *param=NULL;
		char *newlanguage=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param=cstk(l1);
		/* Convert small code to long code fr => fr_FR */
		newlanguage=convertlanguagealias(param);

		if ( !LanguageIsOK(param) && (newlanguage == NULL) )
		{
			if (getWarningMode())
			{
				if (newlanguage == NULL) 
				{
					sciprint(_("%s: Unsupported language '%s'.\n"),fname,param);
				}
				else
				{
					sciprint(_("%s: Unsupported language '%s'.\n"),fname,newlanguage);
				}
			}

			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
			if (newlanguage) { FREE(newlanguage); newlanguage = NULL; }

			LhsVar(1)=Rhs+1;
			PutLhsVar();
			return 0;
		}

		if (newlanguage)
		{
			if (needtochangelanguage(newlanguage))
			{
				if (!setlanguage(newlanguage))
				{
					CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
					*istk(l1)=(int)(FALSE);
				}
				else
				{
					CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
					*istk(l1)=(int)(TRUE);
				}
			}
			else
			{
				/* do nothing */
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(TRUE);
			}
		}
		else
		{
			if (getWarningMode())
			{
				sciprint(_("Unsupported language '%s'.\n"),param);
				sciprint(_("Switching to default language : '%s'.\n"),SCILABDEFAULTLANGUAGE);	
			}
			setlanguage(SCILABDEFAULTLANGUAGE);

			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}
		LhsVar(1)=Rhs+1;
		PutLhsVar();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
