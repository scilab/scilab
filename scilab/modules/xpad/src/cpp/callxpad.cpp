/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "Xpad.hxx"

extern "C"
{
#include "callxpad.h"
#include "charEncoding.h"
#include "getScilabJavaVM.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xpad;
/*--------------------------------------------------------------------------*/
int callXpadW(wchar_t **_wcfilenames, int _nbfiles)
{
	if ( (_wcfilenames) && (_nbfiles > 0) )
	{
		int i = 0;
		for (i = 0; i < _nbfiles; i++)
		{
			if (_wcfilenames[i])
			{
				wchar_t *wcFullFilename = getFullFilenameW(_wcfilenames[i]);
				if (wcFullFilename)
				{
					char *fullFilename = wide_string_to_UTF8(wcFullFilename);
					if (fullFilename)
					{
						Xpad::xpad(getScilabJavaVM(), fullFilename);
						FREE(fullFilename);
						fullFilename = NULL;
					}
					
					FREE(wcFullFilename);
					wcFullFilename = NULL;
				}
			}
		}
	}
	else
	{
		Xpad::xpad(getScilabJavaVM());
	}
	return 0;
}

int callXpadWWithLineNumber(wchar_t **_wcfilenames, double* pdblLineNumber, int _nbfiles)
{
	if ( (_wcfilenames) && (_nbfiles > 0) )
	{
		int i = 0;
		for (i = 0; i < _nbfiles; i++)
		{
			if (_wcfilenames[i])
			{
				wchar_t *wcFullFilename = getFullFilenameW(_wcfilenames[i]);
				if (wcFullFilename)
				{
					char *fullFilename = wide_string_to_UTF8(wcFullFilename);
					if (fullFilename)
					{
						Xpad::xpad(getScilabJavaVM(), fullFilename, (int)pdblLineNumber[i]);
						FREE(fullFilename);
						fullFilename = NULL;
					}
					
					FREE(wcFullFilename);
					wcFullFilename = NULL;
				}
			}
		}
	}
	else
	{
		Xpad::xpad(getScilabJavaVM());
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
