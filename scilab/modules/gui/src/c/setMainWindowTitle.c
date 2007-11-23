/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include "setMainWindowTitle.h"
#include "getScilabJNIEnv.h"
#include "getScilabObject.h"
/*--------------------------------------------------------------------------*/ 
BOOL setMainWindowTitle(char* newtitle)
{
	BOOL bOK = FALSE;
	if (newtitle)
	{
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
							jmethodID mid = (*env)->GetMethodID(env, cls, "setTitle", "(Ljava/lang/String;)V");
							if (mid)
							{
								jstring jstr;
								jstr = (*env )->NewStringUTF(env ,newtitle);
								(*env)->CallVoidMethod(env, jMainView, mid,jstr);
								bOK = TRUE;
							}
						}
					}
				}
			}
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 

