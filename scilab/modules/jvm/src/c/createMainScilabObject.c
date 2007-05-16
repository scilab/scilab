/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "createMainScilabObject.h"
#include "getScilabObject.h"
#include "getScilabJNIEnv.h"
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
		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetMethodID(currentENV,cls,"<init>","(I)V");
			if (mid)
			{
				jint ScilabMode = getScilabMode();
				ScilabObject = (*currentENV)->NewObject(currentENV,cls,mid,ScilabMode); 
				//if (ScilabObject) bOK = TRUE;
				 bOK = TRUE;
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
