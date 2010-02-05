
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __CATCHIFJAVAEXCEPTION_H__
#define __CATCHIFJAVAEXCEPTION_H__

#include "BOOL.h" /* BOOL */
#include "dynlib_jvm.h"

/**
 * Check if there is an exception and print if it is the case
 * @param errorMsg the error message which will be displayed if an exception is catched 
 * @return 
 */
JVM_IMPEXP BOOL catchIfJavaException(char *errorMsg);

#endif /* __CATCHIFJAVAEXCEPTION_H__*/
/*--------------------------------------------------------------------------*/
