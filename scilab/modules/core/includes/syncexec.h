/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SYNCEXEC_H__
#define __SYNCEXEC_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"

/**
* execute a macro in scilab
* @param macro to execute (a string , "a=1+3;")
* @param length of macro (here 6)
* @param code error returned
* @param mode sequential or not
* @param length of macro (again, fortran)
*/
int syncexec(char *str, int *ns, int *ierr, int *seq, long int str_len);

#endif /* __SYNCEXEC_H__ */
/*--------------------------------------------------------------------------*/
