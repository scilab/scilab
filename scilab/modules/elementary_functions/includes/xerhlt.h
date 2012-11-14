/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __XERHLT_H__
#define __XERHLT_H__

#include "machine.h" /* C2F */
#include "dynlib_elementary_functions.h"

/**
* do a long jump (slatec)
* @param messg a message
* @param l length of messg
*/
ELEMENTARY_FUNCTIONS_IMPEXP void C2F(xerhlt) (char *messg, unsigned long l);

/**
* setjmp slatec
* @returns the value 0 if returning directly and  non-zero
* when returning from longjmp() using the saved context
*/
ELEMENTARY_FUNCTIONS_IMPEXP int setjmp_slatec_jmp_env(void);

#endif /* __XERHLT_H__ */
/*--------------------------------------------------------------------------*/
