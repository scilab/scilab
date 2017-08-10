/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * ...
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
#include "gw_differential_equations4.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern int C2F(int2d)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_int2d(char *fname, unsigned long fname_len)
{
    C2F(int2d)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
