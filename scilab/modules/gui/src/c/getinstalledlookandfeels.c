/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include <string.h>
#include "MALLOC.h"
#include "getinstalledlookandfeels.h"
#include "getScilabJNIEnv.h"
/*-----------------------------------------------------------------------------------*/ 
char ** getinstalledlookandfeels(int *sizeReturned)
{
	char **ListLookAndFeels = NULL;
	JNIEnv *env = getScilabJNIEnv();

	if (env)
	{
		jclass classLookAndFeel = (*env)->FindClass(env, "org/scilab/modules/gui/utils/LookAndFeel");
		if (classLookAndFeel)
		{
			jmethodID methodgetInstalledLookAndFeels = (*env)->GetStaticMethodID(env,classLookAndFeel, "getInstalledLookAndFeels", "()[Ljava/lang/String;"); 
			jobjectArray jStrings = (*env)->CallStaticObjectMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels,NULL);

			jobjectArray objStrInstalledLookAndFeels = (*env)->CallStaticObjectMethod(env,classLookAndFeel,methodgetInstalledLookAndFeels);
			*sizeReturned=(*env)->GetArrayLength(env,objStrInstalledLookAndFeels);
			if (*sizeReturned > 0)
			{
				int i=0;
				ListLookAndFeels=(char**)MALLOC(sizeof(char*)*(*sizeReturned));
				for (i=0;i<*sizeReturned;i++)
				{
					jstring jelement;
					const char *str = NULL;
					char *string = NULL;

					jelement = (jstring)(*env)->GetObjectArrayElement(env, jStrings, i);
					str = (*env)->GetStringUTFChars(env,jelement, 0);
					string = (char*)MALLOC(sizeof(char)*(strlen(str)+1));

					strcpy(string,str);

					ListLookAndFeels[i]=string;
					(*env)->ReleaseStringUTFChars(env, jelement, str);
				}
			}
		}
	}
	return ListLookAndFeels;
}
/*-----------------------------------------------------------------------------------*/ 
