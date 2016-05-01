/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "machine.h" /* C2F */
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfort)(char *fname, unsigned long fname_len); /* fortran */
/*--------------------------------------------------------------------------*/
int sci_fort(char *fname, unsigned long fname_len)
{
    return sci_call(fname, fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_call(char *fname, unsigned long fname_len)
{
    //if (Rhs == 0)
    //{
    SciError(39);
    //}
    //else
    //{
    //    C2F(intfort)(fname, fname_len);
    //}
    return 0;
}
/*--------------------------------------------------------------------------*/
