/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre Ledru */
/*-----------------------------------------------------------------------------------*/
#include <jni.h>
#include <string.h>
#include "catchIfJavaException.h"
#include "getScilabJNIEnv.h"
/*-----------------------------------------------------------------------------------*/
BOOL catchIfJavaException(char *errorMsg) 
{
	JNIEnv * currentENV = getScilabJNIEnv();

	if ((*currentENV)->ExceptionCheck(currentENV) == JNI_TRUE)
	{
		printf(errorMsg);
		fflush(NULL);
		(*currentENV)->ExceptionDescribe(currentENV);
		(*currentENV)->ExceptionClear(currentENV);
		return FALSE;
	}
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
