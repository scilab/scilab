/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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
