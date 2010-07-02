/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_data_structures1.h"
/*--------------------------------------------------------------------------*/
extern int C2F(gratyp)(); /* fortran */
extern int C2F(mtlist)(); /* fortran */
/*--------------------------------------------------------------------------*/
int sci_glist(char *fname,unsigned long fname_len)
{
    C2F(mtlist)(C2F(gratyp));
	return 0;
}
/*--------------------------------------------------------------------------*/
