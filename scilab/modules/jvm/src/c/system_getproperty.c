/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include <string.h>
#include "system_getproperty.h"
#include "getScilabJNIEnv.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
char * system_getproperty(char *property,char *defaultproperty)
{
	char *retValue = NULL;
	JNIEnv * currentENV = getScilabJNIEnv();

	if (currentENV)
	{
		jclass cls=NULL;
		cls = (*currentENV)->FindClass(currentENV, "java/lang/System");
		if (cls)
		{
			jmethodID mid=NULL;
			mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getProperty","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			if (mid)
			{
				const char *strValue = NULL;
				jstring jstrProperty;
				jstring jstrDefaultValue;
				jstring jstrValue;
				jstrProperty = (*currentENV)->NewStringUTF(currentENV,property);
				jstrDefaultValue = (*currentENV)->NewStringUTF(currentENV,defaultproperty);

				jstrValue=(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstrProperty,jstrDefaultValue);

				strValue = (*currentENV)->GetStringUTFChars(currentENV,jstrValue, 0);
				if (strValue)
				{
					retValue = (char*)MALLOC(sizeof(char)*(strlen(strValue)+1));
					if (retValue) strcpy(retValue,strValue);
				}
				(*currentENV)->ReleaseStringUTFChars(currentENV, jstrValue , strValue);
			}
		}
	}
	return retValue;
}
/*-----------------------------------------------------------------------------------*/ 
