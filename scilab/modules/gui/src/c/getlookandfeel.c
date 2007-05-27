/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include "getlookandfeel.h"
#include "MALLOC.h"
#include "getScilabJNIEnv.h"
/*-----------------------------------------------------------------------------------*/ 
char * getlookandfeel(void)
{
	char *lookstr = NULL;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeel");
		if (classLookAndFeel)
		{
			jmethodID methodgetLookAndFeel = (*env)->GetStaticMethodID(env,classLookAndFeel, "getLookAndFeel", "()Ljava/lang/String;");
			if (methodgetLookAndFeel)
			{
				jstring jstrValue = (*env)->CallStaticObjectMethod(env,classLookAndFeel, methodgetLookAndFeel,NULL);
				if (jstrValue)
				{
					const char *strValue = NULL;
					strValue = (*env)->GetStringUTFChars(env,jstrValue, 0);
					if (strValue)
					{
						lookstr = (char*)MALLOC(sizeof(char)*(strlen(strValue)+1));
						strcpy(lookstr,strValue);
					}
					(*env)->ReleaseStringUTFChars(env, jstrValue , strValue);
				}
			}
		}
	}
	return lookstr;
}
/*-----------------------------------------------------------------------------------*/ 
