
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include "addToClasspath.h"
#include "getScilabJNIEnv.h"
#include "../../fileio/includes/isdir.h"
#include "../../fileio/includes/FileExist.h"
#include "fromjava.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL addToClasspath(char *classpathstring, typeOfLoad load)
{

	if (classpathstring)
	{
		if ( isdir(classpathstring) || FileExist(classpathstring) )
		{
			JNIEnv * currentENV = getScilabJNIEnv();
			if (currentENV)
			{
				jclass cls=NULL;
				if (IsFromJava())
				{
					/* Boot loader for scilab and javasci */
					/* if scilab is called from java (javasci), we need to update standard classpath */
					/* doesn't require to add -cp SCI/jar/modules/jvm.jar when you use javasci */
					cls = (*currentENV)->FindClass(currentENV, "javasci/ClassPath");
				}
				else
				{
					cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");
				}
				
				if (cls)
				{
					jmethodID mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "addFile","(Ljava/lang/String;I)V");
					if (mid)
					{
						jstring jstr = (*currentENV)->NewStringUTF(currentENV,classpathstring);
						(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstr, (int)load);
						return TRUE;
					}
				}
			}
		}else{
			fprintf(stderr,_("Warning: Could not find Java package '%s'.\n"),classpathstring);
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
