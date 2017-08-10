/*
 * ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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

#include "do_error_number.h"
#include "errorTable.h"
#include "lasterror.h"
/*--------------------------------------------------------------------------*/
int C2F(error)(int *n)
{
    return 0;
}
/*--------------------------------------------------------------------------*/
void SciError(int n)
{
    C2F(error)(&n);
}
/*--------------------------------------------------------------------------*/
void SciStoreError(int n)
{
    // FIXME
}
/*--------------------------------------------------------------------------*/
