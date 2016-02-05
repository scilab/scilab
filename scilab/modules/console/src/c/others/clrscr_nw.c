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
