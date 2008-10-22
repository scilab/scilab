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
#include <jni.h>
#include "addToLibrarypath.h"
#include "getScilabJNIEnv.h"
#include "../../fileio/includes/isdir.h"
#include "fromjava.h"
#
/*--------------------------------------------------------------------------*/ 
BOOL addToLibrarypath(char *librarypathstring)
{

	if (librarypathstring)
	{
		if ( isdir(librarypathstring) )
		{
			JNIEnv * currentENV = getScilabJNIEnv();
			if (currentENV)
			{
				jclass cls=NULL;
				if (IsFromJava())
				{
					/* Boot loader for scilab and javasci */
					/* if scilab is called from java (javasci), we need to update standard java.library.path */
					/* doesn't require to add -cp SCI/jar/modules/jvm.jar when you use javasci */
					cls = (*currentENV)->FindClass(currentENV, "javasci/LibraryPath");
				}
				else
				{
					cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/LibraryPath");
				}
				
				if (cls)
				{
					jmethodID mid=NULL;
					mid = (*currentENV)->GetStaticMethodID(currentENV, cls,"addPath","(Ljava/lang/String;)V");
					if (mid)
					{
						jstring jstr;
						jstr = (*currentENV)->NewStringUTF(currentENV,librarypathstring);
						(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstr);
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
