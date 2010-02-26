
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GETSCILABJNIENV_H__
#define __GETSCILABJNIENV_H__

#include <jni.h> /* JNIEnv */

/**
* returns Scilab JNIEnv
* @return JNIEnv
*/
JNIEnv *getScilabJNIEnv(void);

#endif /* __GETSCILABJAVAVM_H__ */
/*--------------------------------------------------------------------------*/ 
