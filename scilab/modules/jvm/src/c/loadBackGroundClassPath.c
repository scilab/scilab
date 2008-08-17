/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Sylvestre LEDRU
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
#include <stdio.h>
#include "loadBackGroundClassPath.h"
#include "addToClasspath.h"
#include "getScilabJNIEnv.h"
#include "fromjava.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL loadBackGroundClassPath(void)
{
	JNIEnv * currentENV = getScilabJNIEnv();
	if (currentENV)
		{
			jclass cls=NULL;
			cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");

			if (cls)
				{
					jmethodID mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "loadBackGroundClassPath","()V");
					if (mid)
						{
							(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid);
							return TRUE;
						}
				}
		}
	
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
