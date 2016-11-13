/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
COMPLETION_IMPEXP char *completeLine(char *currentline, char *stringToAdd, char *filePattern,
                                     char *defaultPattern, BOOL stringToAddIsPath, char *postCaretLine);

#endif /* __COMPLETE_LINE_H__ */
