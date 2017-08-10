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
