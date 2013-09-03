/*
 * ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __ERROR_TABLE_H__
#define __ERROR_TABLE_H__

/**
* internal error table
* see also help error_table in scilab
* @param[in] error code
* @return value returned by internal Scierror
*/
int errorTable(int iErr);

#endif /*  __ERROR_TABLE_H__ */