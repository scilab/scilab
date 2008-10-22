/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "setgetlanguage.h"
#include "Scierror.h"
#include "sciprint.h"
#include "warningmode.h"
#include "saveLanguagePref.h"
#include "LanguagePref.h"
#include "deleteafile.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
char *getLanguageFromAlias(char *alias)
{
	if (alias)
	{
		if ( strcmp(alias,"en") == 0 )
		{
			return strdup("en_US");
		}

		if ( strcmp(alias,"fr") == 0 )
		{
			return strdup("fr_FR");
		}

		return strdup(alias);
	}
	/* "" value fixed by system */
	else return strdup("");
}
/*--------------------------------------------------------------------------*/
BOOL isValidLanguage(char *lang)
{
	/* Try to detect if it is a correct language */
	if (lang)
	{
		if ( strcmp(lang,"") == 0) return TRUE;
		if ( strcmp(lang,"C") == 0) return TRUE;
		if ( strcmp(lang,"en") == 0) return TRUE;
		if ( strcmp(lang,"fr") == 0) return TRUE;
		if ( ((int) strlen(lang) == 5) && (lang[2] == '_') ) return TRUE; /* xx_XX */
	}
	return FALSE;

}
/*--------------------------------------------------------------------------*/
int sci_setdefaultlanguage(char *fname,unsigned long fname_len)
{
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		static int l1 = 0, n1 = 0, m1 = 0;
		char *newlang = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

		newlang = getLanguageFromAlias(cstk(l1));

		if ( !isValidLanguage(newlang) )
		{
			if (getWarningMode())
			{
				sciprint(_("Unsupported language '%s'.\n"),newlang);
			}

			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
			if (newlang) { FREE(newlang); newlang = NULL; }

			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
			return 0;
		}

		if ( strcmp(newlang,getlanguage()) == 0 )
		{
			/* do nothing */
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			if ( !setlanguage(newlang,FALSE,FALSE) )
			{
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1)=(int)(FALSE);
			}
			else
			{
				if ( getWarningMode() )
				{
					sciprint("\n");
					sciprint(_("The language for menus cannot be changed on a running console.\n"));
					sciprint(_("Restart Scilab to apply to menus.\n"));
				}
				
				CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				if ( strcmp(newlang,"") )
				{
					*istk(l1)=(int)saveLanguagePref();
				}
				else /* "" language fixed by the system */
				{
					char *FileFilenameLanguagePref = getFilenameLanguagePref();
					if (FileFilenameLanguagePref)
					{
						/* remove preference file */
						deleteafile(FileFilenameLanguagePref);
						FREE(FileFilenameLanguagePref);
						FileFilenameLanguagePref = NULL;
					}

					*istk(l1)=(int)(TRUE);
				}
			}
		}

		if (newlang) { FREE(newlang); newlang = NULL; }

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for first input argument: String expected.\n"),fname);
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/

