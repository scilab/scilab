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
#include <jni.h>
#include <string.h>
#include "catchIfJavaException.h"
#include "getScilabJNIEnv.h"
/*--------------------------------------------------------------------------*/
BOOL catchIfJavaException(char *errorMsg) 
{
	JNIEnv * currentENV = getScilabJNIEnv();

	if ((*currentENV)->ExceptionCheck(currentENV) == JNI_TRUE)
	{
		printf("%s",errorMsg);
		fflush(NULL);
		(*currentENV)->ExceptionDescribe(currentENV);
		(*currentENV)->ExceptionClear(currentENV);
		return FALSE;
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/
