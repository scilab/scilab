/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "deleteafile.h"
#ifndef _MSC_VER
	#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
	#include <Windows.h>
#endif
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL deleteafile(char *filename)
{
	BOOL bOK = FALSE;
	#ifndef _MSC_VER
	{
		FILE *f = fopen(filename, "r") ;
		if (! f) return bOK;
		fclose(f) ;
		chmod(filename, S_IWRITE) ;
		if (remove(filename) == 0)  bOK = TRUE;
	}
	#else
	{
		if (filename)
		{
			wchar_t *wcfilename = to_wide_string(filename);
			if (wcfilename)
			{
				bOK = deleteafileW(wcfilename);
				FREE(wcfilename);
				wcfilename = NULL;
			}
		}
	}
	#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL deleteafileW(wchar_t *filenameW)
{
	BOOL bOK = FALSE;
#ifndef _MSC_VER
	{
		char *filename = wide_string_to_UTF8(filenameW);
		if (filename)
		{
			bOK = deleteafile(filename);
			FREE(filename);
			filename = NULL;
		}
	}
#else
	if (filenameW)
	{
		bOK = DeleteFileW(filenameW);
	}
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
