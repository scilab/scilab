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

#ifndef __COMPLETE_LINE_H__
#define __COMPLETE_LINE_H__

#include "dynlib_completion.h"
#include "BOOL.h"


/**
* complete line with a string 
* @param[in] current line 
* @param[in] string to add
* @param[in] file pattern
* @param[in] default pattern
* @param[in] string added is path or not
* @param[in] string after caret (completion with insertion)
* @return line assembled
*/
COMPLETION_IMPEXP char *completeLine(char *currentline,char *stringToAdd,char *filePattern,
				   char *defaultPattern,BOOL stringToAddIsPath, char *postCaretLine);

#endif /* __COMPLETE_LINE_H__ */
