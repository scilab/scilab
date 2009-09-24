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

#ifndef __BASENAME_H__
#define __BASENAME_H__

#include <wchar.h>
#include "BOOL.h"


/**
* strip directory and suffix from filenames
* @param[in] filename
* @param[in] BOOL flagexpand
* @return basename
*/
char *basename(char *fullfilename, BOOL bExpand);


/**
* strip directory and suffix from filenames (wide string)
* @param[in] filename
* @param[in] BOOL flagexpand
* @return basename
*/
wchar_t *basenameW(wchar_t *wcfullfilename, BOOL bExpand);

#endif