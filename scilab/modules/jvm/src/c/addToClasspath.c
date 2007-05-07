/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "addToClasspath.h"
#include <jni.h>
#include "getScilabJNIEnv.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL addToClasspath(char *classpathstring)
{
	BOOL bOK=FALSE;

	if (classpathstring)
	{
		JNIEnv * currentENV = getScilabJNIEnv();

		jclass cls=NULL;
		jmethodID mid=NULL;

		if (currentENV)
		{
			cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");
			if (cls)
			{
				mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "addFile","(Ljava/lang/String;)V");
				if (mid)
				{
					jstring jstr;
					jstr = (*currentENV)->NewStringUTF(currentENV,classpathstring);
					(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstr);
					bOK=TRUE;
				}
			}
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
