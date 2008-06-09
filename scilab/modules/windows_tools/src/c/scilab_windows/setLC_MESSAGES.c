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
#include <windows.h>
#include <strsafe.h>
#include "win_mem_alloc.h" /* MALLOC */
#include "scilabDefaults.h"
/*--------------------------------------------------------------------------*/
/**
* Initialize LC_MESSAGES environment variable
* To fix a bug with Windows multilanguage (MUI)
* see J-B and Simoné pcs 
* english GUI & french keyboard
* with French GUI installed but NOT used
*/
/*--------------------------------------------------------------------------*/
void setLC_MESSAGES(void)
{
	#define LENGTH_BUFFER 1024
	#define FORMAT_LOCALE "%s_%s"
	char buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
	char buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
	char *localeStr = NULL;
	int ret = 0;
	ret = GetLocaleInfoA(LOCALE_USER_DEFAULT,
						LOCALE_SISO639LANGNAME,
						&buffer_LOCALE_SISO639LANGNAME[0],
						LENGTH_BUFFER);
	if (ret > 0)
	{
		ret = GetLocaleInfoA(LOCALE_USER_DEFAULT,
							LOCALE_SISO3166CTRYNAME,
							&buffer_LOCALE_SISO3166CTRYNAME[0],
							LENGTH_BUFFER);
		if (ret >0)
		{
			int length_localeStr = (int)(strlen(buffer_LOCALE_SISO639LANGNAME)+
										 strlen(buffer_LOCALE_SISO3166CTRYNAME)+
										 strlen(FORMAT_LOCALE));
			localeStr = (char*)MALLOC(sizeof(char)*(length_localeStr)+1);
			if (localeStr)
			{
				StringCchPrintfA(localeStr,length_localeStr,FORMAT_LOCALE,
					buffer_LOCALE_SISO639LANGNAME,
					buffer_LOCALE_SISO3166CTRYNAME);
				SetEnvironmentVariableA(EXPORTENVLOCALESTR,localeStr);
				FREE(localeStr);
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
