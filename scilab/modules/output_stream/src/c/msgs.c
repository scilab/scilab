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
