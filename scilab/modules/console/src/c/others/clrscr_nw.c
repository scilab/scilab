/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "clrscr_nw.h"
#include "gotoxy_nw.h"
/*--------------------------------------------------------------------------*/
#define CLRSCR_NW_FORMAT "\033[2J"
/*--------------------------------------------------------------------------*/
void clrscr_nw(void)
{
    printf(CLRSCR_NW_FORMAT);
    gotoxy_nw(0, 0);
}
/*--------------------------------------------------------------------------*/
