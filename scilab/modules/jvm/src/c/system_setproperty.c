/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include <string.h>
#include "system_setproperty.h"
#include "getScilabJNIEnv.h"
#include "MALLOC.h"
#include "catchIfJavaException.h"
/*--------------------------------------------------------------------------*/ 
char * system_setproperty(char *property,char *value)
{
	char *retValue = NULL;

	JNIEnv * currentENV = getScilabJNIEnv();

	if (currentENV)
	{
		jclass cls = NULL;
		cls = (*currentENV)->FindClass(currentENV, "java/lang/System");
		if (cls)
		{
			jmethodID mid = NULL;
			mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "setProperty","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			if (mid)
			{
				BOOL bOK = FALSE;
				const char *strPreviousValue = NULL;
				jstring jstrProperty;
				jstring jstrValue;
				jstring jstrPreviousValue;
		
				jstrProperty = (*currentENV)->NewStringUTF(currentENV,property);
				jstrValue = (*currentENV)->NewStringUTF(currentENV,value);

				jstrPreviousValue = (*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstrProperty,jstrValue);
				bOK = catchIfJavaException(""); 
				if (bOK)
				{
					strPreviousValue = (*currentENV)->GetStringUTFChars(currentENV,jstrPreviousValue, 0);
					if (strPreviousValue)
					{
						retValue = (char*)MALLOC(sizeof(char)*(strlen(strPreviousValue)+1));
						if (retValue) strcpy(retValue,strPreviousValue);
					}
					(*currentENV)->ReleaseStringUTFChars(currentENV, jstrPreviousValue , strPreviousValue);
				}
			}
		}
	}
	return retValue;
}
/*--------------------------------------------------------------------------*/ 
