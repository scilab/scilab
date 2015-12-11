/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "configvariable_interface.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "msgs.h"
#include "localization.h"
#include "cvstr.h"
#include "do_error_number.h"
/*--------------------------------------------------------------------------*/
static int msg_default(int *n, int *ierr);
/*--------------------------------------------------------------------------*/
void Msgs(int n, int ierr)
{
    C2F(msgs)(&n, &ierr);
}
/*--------------------------------------------------------------------------*/
int C2F(msgs)(int *n, int *ierr)
{
    sciprint("\n");
    return 0;
}
