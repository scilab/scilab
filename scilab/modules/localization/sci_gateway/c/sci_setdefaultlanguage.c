/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_localization.h"
#include "stack-c.h"
#include "warningmode.h"
#include "localization.h"
#include "sciprint.h"
#ifdef _MSC_VER
#include "MALLOC.h"
#include "Scierror.h"
#include "strdup_windows.h"
#include "BOOL.h"
#include "setgetlanguage.h"
#include "LanguagePreferences_Windows.h"
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char *getLanguageFromAlias(char *alias);
#endif
/*--------------------------------------------------------------------------*/
int sci_setdefaultlanguage(char *fname,unsigned long fname_len)
{
	int n1 = 0, m1 = 0, l1 = 0;

	CheckRhs(1, 1);
	CheckLhs(1, 1);
	#ifndef _MSC_VER
	{
		if (getWarningMode())
		{
			sciprint(_("%s: This feature is only used on Windows.\n"), fname);
		}
		m1 = 1; n1 = 1;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
		*istk(l1)=(int)(FALSE);

		LhsVar(1) = Rhs+1;
		PutLhsVar();
	}
	#else
	{
		if (GetType(1) == sci_strings)
		{
			char *newlang = NULL;
			GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
			newlang = getLanguageFromAlias( cstk(l1) );

			if ( !isValidLanguage(newlang) )
			{
				if ( getWarningMode() )
				{
					sciprint(_("Unsupported language '%s'.\n"),newlang);
				}
				m1 = 1; n1 = 1;
				CreateVar(Rhs+1, MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
				*istk(l1) = (int)(FALSE);
                if (newlang) { FREE(newlang); newlang = NULL; }
				LhsVar(1) = Rhs+1;
				PutLhsVar();
				return 0;
			}
			else
			{
				char *savedLanguage = getLanguagePreferences();
				if ( strcmp(newlang, savedLanguage) == 0 )
				{
					/* do nothing */
					m1 = 1; n1 = 1;
					CreateVar(Rhs+1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &l1);
					*istk(l1) = (int)(TRUE);
					LhsVar(1) = Rhs+1;
					if (newlang) { FREE(newlang); newlang = NULL; }
					if (savedLanguage) { FREE(savedLanguage); savedLanguage = NULL; }
					PutLhsVar();
					return 0;
				}
				else
				{
					if (savedLanguage) { FREE(savedLanguage); savedLanguage = NULL; }
					if ( !setlanguage(newlang) ) /* */
					{
						m1 = 1; n1 = 1;
						CreateVar(Rhs+1, MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
						*istk(l1) = (int)(FALSE);
						LhsVar(1) = Rhs+1;
                        if (newlang) { FREE(newlang); newlang = NULL; }
						PutLhsVar();

						return 0;
					}
					else
					{
						if ( getWarningMode() )
						{
							sciprint("\n");
							sciprint(_("The language for menus cannot be changed on a running console.\n"));
							sciprint(_("Restart Scilab to apply to menus.\n"));
						}

						m1 = 1; n1 = 1;
						CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);

						if ( setLanguagePreferences() ) *istk(l1) = (int)TRUE;
						else *istk(l1) = (int)FALSE;
						if (newlang) { FREE(newlang); newlang = NULL; }
						LhsVar(1) = Rhs+1;
						PutLhsVar();
						return 0;
					}
				}
			 }
		}
		else
		{
			Scierror(999,_("%s: Wrong type for first input argument: String expected.\n"), fname);
		}
	}
	#endif

	return 0;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char *getLanguageFromAlias(char *alias)
{
	if (alias)
	{
		if ( strcmp(alias, "en") == 0 )
		{
			return strdup("en_US");
		}

		if ( strcmp(alias, "fr") == 0 )
		{
			return strdup("fr_FR");
		}

		return strdup(alias);
	}

	/* "" value fixed by system */
	return strdup("");
}
/*--------------------------------------------------------------------------*/
#endif
