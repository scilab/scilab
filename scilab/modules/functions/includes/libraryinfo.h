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
#ifndef __LIBRARYINFO_H__
#define __LIBRARYINFO_H__

#include "dynlib_functions.h"

/**
* get path of scilab library
* @param[in] name of scilab library
* @return a string : path of library
*/
FUNCTIONS_SCILAB_IMPEXP char *getlibrarypath(char *libraryname);

/**
* get macros list in a scilab's library
* @param[in] name of scilab library
* @param[out] size of array returned
* @return an array of string (macros list)
*/
FUNCTIONS_SCILAB_IMPEXP char **getlistmacrosfromlibrary(char *libraryname, int *sizearray);

#endif /* __LIBRARYINFO_H__ */
/*--------------------------------------------------------------------------*/
