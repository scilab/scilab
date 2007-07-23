/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "createMainScilabObject.h"
#include "getScilabObject.h"
#include "getScilabJNIEnv.h"
#include "getScilabJavaVM.h"
#include "catchIfJavaException.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
static jobject ScilabObject;
/*-----------------------------------------------------------------------------------*/ 
BOOL createMainScilabObject(void)
{
	BOOL bOK = FALSE;
	JNIEnv * currentENV = getScilabJNIEnv();

	if (currentENV)
	{
		jclass cls=NULL;
		cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/Scilab");
			bOK=catchIfJavaException("Could not access to the Main Scilab Class :\n");
		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetMethodID(currentENV,cls,"<init>","(I)V");
			bOK=catchIfJavaException("Could not access the creator of the Main Scilab Class :\n");
			if (mid)
			{
				jint ScilabMode = getScilabMode();
				ScilabObject = (*currentENV)->NewObject(currentENV,cls,mid,ScilabMode); 
				/* Catch the exception and display an human-reading error message 
				 */
				bOK=catchIfJavaException("Could not create a Scilab main class. Error :\n");
			}
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
jobject getScilabObject(void)
{
	return ScilabObject;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL finishMainScilabObject(void)
{
	BOOL bOK = FALSE;
	JNIEnv * currentENV = getScilabJNIEnv();
	JavaVM * currentJVM = getScilabJavaVM();

	(*currentJVM)->AttachCurrentThread( currentJVM, (void **) &currentENV , NULL ) ;
	(*currentENV)->DeleteGlobalRef(currentENV, ScilabObject);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
