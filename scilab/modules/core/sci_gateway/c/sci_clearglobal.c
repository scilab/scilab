/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_core.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intclearglobal)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_clearglobal)(char *fname, unsigned long fname_len)
{
    C2F(intclearglobal)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
