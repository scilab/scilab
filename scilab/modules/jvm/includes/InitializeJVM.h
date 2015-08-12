
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

/*--------------------------------------------------------------------------*/
#ifndef __INITIALIZEJVM_H__
#define __INITIALIZEJVM_H__

#include "dynlib_jvm.h"
#include "BOOL.h"

/**
* Initialize JVM
* @return TRUE or FALSE
*/
JVM_IMPEXP BOOL InitializeJVM(void);

/**
* Execute initial hooks register in the class Scilab.java
* @return TRUE or FALSE
*/
JVM_IMPEXP BOOL ExecuteInitialHooks(void);

#endif /* __INITIALIZEJVM_H__ */
/*--------------------------------------------------------------------------*/
