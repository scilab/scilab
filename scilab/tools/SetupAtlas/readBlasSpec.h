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
/*--------------------------------------------------------------------------*/
#ifndef __READBLASSPEC_H__
#define __READBLASSPEC_H__

#include <wchar.h>

struct cpu_struct
{
	wchar_t *cpu_vendor;
	int cpu_family;
	int cpu_model;
	wchar_t *dll_filename;
	wchar_t *comments;
};

struct cpu_struct ** readBlasSpec(wchar_t *blasspec_filename,
								  int *sizeArrayReturned);

#endif /* __READBLASSPEC_H__ */
/*--------------------------------------------------------------------------*/