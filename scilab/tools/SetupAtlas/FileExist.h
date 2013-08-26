/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __FILEEXIST_H__
#define __FILEEXIST_H__

#include <wchar.h>

/**
* checks if a file exists
* @param[in] filename
* @return TRUE if exist
*/
BOOL FileExist(wchar_t  *wcfilename);

#endif /* __FILEEXIST_H__ */
/*--------------------------------------------------------------------------*/
