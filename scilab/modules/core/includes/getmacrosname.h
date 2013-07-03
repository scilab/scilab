/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GETMACROSNAME_H__
#define __GETMACROSNAME_H__

/**
* get scilab macros list (sorted by name)
* macros on scilab and in libraries
* @param[out] size of returned array
* @return array of strings
*/
char **getMacrosName(int *sizearray);

#endif /* __GETMACROSNAME_H__ */
/*--------------------------------------------------------------------------*/
