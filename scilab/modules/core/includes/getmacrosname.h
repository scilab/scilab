/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - 2010 - DIGITEO - Allan CORNET
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
