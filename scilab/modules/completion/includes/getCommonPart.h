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

/*--------------------------------------------------------------------------*/
#ifndef __GETCOMMONPART_H__
#define __GETCOMMONPART_H__

#include "dynlib_completion.h"

/**
* get common part of words
* example gethistory, gethistoryfile, gethist
* it will return : gethist
* @param[in] matrix of strings
* @param[in] size of array
* @return a string common part
*/
COMPLETION_IMPEXP char *getCommonPart(char **dictionary, int sizeDictionary);

#endif /* __GETCOMMONPART_H__ */

/*--------------------------------------------------------------------------*/
