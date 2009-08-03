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
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xpad;
/*--------------------------------------------------------------------------*/
int callXpad(char **_filenames, int _nbfiles)
{
	if ( (_filenames) && (_nbfiles > 0) )
	{
		int i = 0;
		for (i = 0; i < _nbfiles; i++)
		{
			Xpad::xpad(getScilabJavaVM(), _filenames[i]);
		}
	}
	else
	{
		Xpad::xpad(getScilabJavaVM());
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int callXpadW(wchar_t **_wcfilenames, int _nbfiles)
{
	if ( (_wcfilenames) && (_nbfiles > 0) )
	{
		int i = 0;
		char **filesname = (char**)MALLOC(sizeof(wchar_t*) * _nbfiles);
		if (filesname)
		{
			for (i = 0; i < _nbfiles; i++)
			{
				filesname[i] = wide_string_to_UTF8(_wcfilenames[i]);
			}
			callXpad(filesname, _nbfiles);
			freeArrayOfString(filesname, _nbfiles);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
