/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#ifndef __GETFILESDICTIONARY_H__
#define __GETFILESDICTIONARY_H__

#include "BOOL.h"

/**
* get files dictionary
* @param[in] some chars
* @param[out] size of returned array
* @param[in] returns with the full path name
* @return array of strings
*/
char **getfilesdictionary(char *somechars, int *sizearray, BOOL fullpath);

#endif /* __GETFILESDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/
