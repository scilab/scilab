/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include <string.h>
#include "getMainWindowTitle.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
char * getMainWindowTitle(void)
{
	char *title = NULL;

	JNIEnv *env = getScilabJNIEnv();
	jobject  ScilabObj = getScilabObject();

	if (env)
	{
		jclass class_Mine = (*env)->GetObjectClass(env, ScilabObj);
		if (class_Mine)
		{
			jfieldID id_Window =  (*env)->GetFieldID(env, class_Mine, "mainView","Lorg/scilab/modules/gui/window/Window;");

			if (id_Window)
			{
				jobject jMainView = (*env)->GetObjectField(env, ScilabObj, id_Window);
				if (jMainView)
				{
					jclass cls = (*env)->GetObjectClass(env, jMainView);
					if (cls)
					{
						jmethodID mid = (*env)->GetMethodID(env, cls, "getTitle", "()Ljava/lang/String;");
						if (mid)
						{
							jstring jstr;
							jstr = (*env)->CallObjectMethod(env, jMainView, mid);
							if (jstr)
							{
								const char *strValue = NULL;
								strValue = (*env)->GetStringUTFChars(env,jstr, 0);
								if (strValue)
								{
									title = (char*)MALLOC(sizeof(char)*(strlen(strValue)+1));
									if (title) strcpy(title,strValue);
								}
								(*env)->ReleaseStringUTFChars(env, jstr , strValue);
							}
						}
					}
				}
			}
		}
	}

	return title;
}
/*--------------------------------------------------------------------------*/ 
