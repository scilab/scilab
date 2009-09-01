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

#ifndef __GETPARTLINE_H__
#define __GETPARTLINE_H__

#include "dynlib_completion.h"

/**
* pre parsing current line to search with completion
* @param[in] current line
* @return word to find with completion
*/
COMPLETION_IMPEXP char *getPartLevel(char *line);

/**
* pre parsing current line to search with completion (file)
* @param[in] current line
* @return word to find with completion
*/
COMPLETION_IMPEXP char *getFilePartLevel(char *line);


#endif /* __GETPARTLINE_H__ */
