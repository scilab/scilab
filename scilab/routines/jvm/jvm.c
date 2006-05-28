/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "jvm.h"
/*-----------------------------------------------------------------------------------*/ 
static JavaVMOption options[2];
static JavaVMInitArgs vm_args;
static JavaVM *jvm;
static JNIEnv *env;
/*-----------------------------------------------------------------------------------*/ 
int StartJVM(void)
{
	int bOK=FALSE;
	long status;

	options[0].optionString = "-Djava.class.path=.";
	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_2;
	vm_args.nOptions = 1;
	vm_args.options = options;
	status = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
	if (status == JNI_ERR)
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
int TerminateJVM(void)
{
	int bOK=FALSE;
	(*jvm)->DestroyJavaVM(jvm);
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 