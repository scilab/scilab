
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
#include "createMainScilabObject.h"
#include "getScilabObject.h"
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "catchIfJavaException.h"
#include "scilabmode.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
static jobject ScilabObject;
/*--------------------------------------------------------------------------*/ 
BOOL createMainScilabObject(void)
{
	BOOL bOK = FALSE;
	JNIEnv * currentENV = getScilabJNIEnv();

	if (currentENV)
	{
		jclass cls=NULL;
		cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/Scilab");
		bOK=catchIfJavaException(_("Could not access to the Main Scilab Class:\n"));
		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetMethodID(currentENV,cls,"<init>","(I)V");
			bOK=catchIfJavaException(_("Could not access to the constructor of the Main Scilab Class:\n"));
			if (mid)
			{
				jint ScilabMode = getScilabMode();
				ScilabObject = (*currentENV)->NewObject(currentENV,cls,mid,ScilabMode); 
				/* Catch the exception and display an human-reading error message 
				 */
				bOK=catchIfJavaException(_("Could not create a Scilab main class. Error:\n"));
			}
		}
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/ 
jobject getScilabObject(void)
{
	return ScilabObject;
}
/*--------------------------------------------------------------------------*/ 
BOOL finishMainScilabObject(void)
{
	JNIEnv * currentENV = getScilabJNIEnv();
	JavaVM * currentJVM = getScilabJavaVM();

	jint result = (*currentJVM)->AttachCurrentThread( currentJVM, (void **) &currentENV , NULL ) ;
	if (result == 0) 
	{
		(*currentENV)->DeleteLocalRef(currentENV, ScilabObject);
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
