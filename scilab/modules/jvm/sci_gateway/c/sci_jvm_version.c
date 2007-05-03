/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "getScilabJVM.h"
#include "jni.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_jvm_version) _PARAMS((char *fname,unsigned long fname_len))
{
	/*
	JavaVM* currentJVM =getScilabJVM();
	JNIEnv * currentENV ;

	jint res;

	jobject JvmVersionObject;
	jclass cls=NULL;
	jmethodID mid=NULL;


	res = (*currentJVM)->GetEnv(currentJVM, (void **)&currentENV, JNI_VERSION_1_6);

	cls = (*currentENV)->FindClass(currentENV, "JvmVersion");

	mid = (*currentENV)->GetMethodID(currentENV, cls, "getVersion","()V");

	JvmVersionObject = (*currentENV)->NewObject(currentENV,cls,mid);

	(*currentENV)->CallObjectMethod(currentENV,(jobject)JvmVersionObject, mid,NULL);

*/
	LhsVar(0) = 0;
	C2F(putlhsvar)();	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
