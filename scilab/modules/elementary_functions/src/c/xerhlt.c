/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#if defined(__linux__)
#define __USE_FORTIFY_LEVEL 0 /* Avoid dependency on GLIBC_2.11 (__longjmp_chk) */
#endif
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
