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
#include <string.h>
#include "getClasspath.h"
#include "getScilabJNIEnv.h"
#include "fromjava.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
char **getClasspath(int *sizeClasspathArray)
{
	char **ClasspathArray=NULL;
	JNIEnv * currentENV = getScilabJNIEnv();
	if (currentENV)
	{
		jclass cls=NULL;
		if (IsFromJava())
		{
			/* Boot loader for scilab and javasci */
			/* if scilab is called from java (javasci), we need to update standard classpath */
			/* doesn't require to add -cp SCI/modules/jvm/jar/org.scilab.modules.jvm.jar when you use javasci */
			cls = (*currentENV)->FindClass(currentENV, "javasci/ClassPath");
		}
		else
		{
			cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");
		}

		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getClassPath","()[Ljava/lang/String;");
			if (mid)
			{
				jobjectArray jStrings=(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,NULL);
				*sizeClasspathArray=(*currentENV)->GetArrayLength(currentENV,jStrings);
				if (*sizeClasspathArray>0)
				{
					int i=0;

					ClasspathArray=(char**)MALLOC(sizeof(char*)*(*sizeClasspathArray));
					for (i=0;i<*sizeClasspathArray;i++)
					{
						jstring jelement;
						const char *str = NULL;
						char *string = NULL;

						jelement = (jstring)(*currentENV)->GetObjectArrayElement(currentENV, jStrings, i);
						str = (*currentENV)->GetStringUTFChars(currentENV,jelement, 0);
						if (str)
						{
							string = (char*)MALLOC(sizeof(char)*(strlen(str)+1));
							if (string)
							{
								#if _MSC_VER
								if (strlen(str) > 1) 
								{
									if (str[0] == '/') strcpy(string,&str[1]);
									else strcpy(string,str);
								}
								else 
								#endif
								{
									strcpy(string,str);
								}
							}
						}
						ClasspathArray[i] = string;
						(*currentENV)->ReleaseStringUTFChars(currentENV, jelement, str);
					}
				}
			}
		}
	}
	return ClasspathArray;
}
/*--------------------------------------------------------------------------*/ 
