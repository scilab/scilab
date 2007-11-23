/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <jni.h>
#include "addToLibrarypath.h"
#include "getScilabJNIEnv.h"
#include "../../fileio/includes/isdir.h"
#include "fromjava.h"
#
/*--------------------------------------------------------------------------*/ 
BOOL addToLibrarypath(char *librarypathstring)
{
	BOOL bOK=FALSE;

	if (librarypathstring)
	{
		if ( isdir(librarypathstring) )
		{
			JNIEnv * currentENV = getScilabJNIEnv();
			if (currentENV)
			{
				jclass cls=NULL;
				if (IsFromJava())
				{
					/* Boot loader for scilab and javasci */
					/* if scilab is called from java (javasci), we need to update standard java.library.path */
					/* doesn't require to add -cp SCI/jar/modules/jvm.jar when you use javasci */
					cls = (*currentENV)->FindClass(currentENV, "javasci/LibraryPath");
				}
				else
				{
					cls = (*currentENV)->FindClass(currentENV, "org/scilab/modules/jvm/LibraryPath");
				}
				
				if (cls)
				{
					jmethodID mid=NULL;
					mid = (*currentENV)->GetStaticMethodID(currentENV, cls,"addPath","(Ljava/lang/String;)V");
					if (mid)
					{
						jstring jstr;
						jstr = (*currentENV)->NewStringUTF(currentENV,librarypathstring);
						(*currentENV)->CallStaticObjectMethod(currentENV,cls, mid,jstr);
						bOK=TRUE;
					}
				}
			}
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
