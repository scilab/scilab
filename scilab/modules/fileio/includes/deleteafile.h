/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __DELETEAFILE_H__
#define __DELETEAFILE_H__

#include <wchar.h>
#include "BOOL.h"

/**
* delete a file by filename
* @param[in] filename
* @return BOOLEAN TRUE or FALSE
*/
BOOL deleteafile(char *filename);

/**
* delete a file by filename 
* @param[in] filenameW wide string
* @return BOOLEAN TRUE or FALSE
*/
BOOL deleteafileW(wchar_t *filenameW);

#endif /* __DELETEAFILE_H__ */
/*--------------------------------------------------------------------------*/
