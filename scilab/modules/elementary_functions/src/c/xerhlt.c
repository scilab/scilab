/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include <string.h>
#include <setjmp.h>
#include "xerhlt.h"
/*--------------------------------------------------------------------------*/
jmp_buf slatec_jmp_env;
/*--------------------------------------------------------------------------*/
void C2F(xerhlt) (char *messg, unsigned long l)
{
    longjmp(slatec_jmp_env, 1);
}
/*--------------------------------------------------------------------------*/
int setjmp_slatec_jmp_env(void)
{
    return setjmp(slatec_jmp_env);
}
/*--------------------------------------------------------------------------*/
