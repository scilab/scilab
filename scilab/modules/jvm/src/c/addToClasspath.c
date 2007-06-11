/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <jni.h>
#include "addToClasspath.h"
#include "getScilabJNIEnv.h"
#include "../../io/includes/directories.h"
#include "../../fileio/includes/FileExist.h"
#include "fromjava.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL addToClasspath(char *classpathstring)
{
	BOOL bOK=FALSE;

	if (classpathstring)
	{
		if ( isdir(classpathstring) || FileExist(classpathstring) )
		{
			JNIEnv * currentENV = getScilabJNIEnv();
			if (currentENV)
			{
				jclass cls=NULL;
				if (IsFromJava())
				{
					/* Boot loader for scilab and javasci */
					/* if scilab is called from java (javasci), we need to update standard classpath */
					/* doesn't require to add -cp SCI/jar/modules/jvm.jar when you use javasci */
					cls = (*currentENV)->FindClass(currentENV, "javasci/ClassPath");
				}
				else
				{
					cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/ClassPath");
				}
				
				if (cls)
				{
					jmethodID mid=NULL;
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
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
