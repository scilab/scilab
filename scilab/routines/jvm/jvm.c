/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "jvm.h"
/*-----------------------------------------------------------------------------------*/ 
static JavaVM *jvm=NULL;
static JNIEnv *env=NULL;
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
static HINSTANCE hLibJVM = NULL;
typedef  jint (JNICALL * JNI_CreateJavaVMPROC) (JavaVM **, void **, void *);
#endif
void JVMCallStaticVoidMethod(char *Class,char *Method);

/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
jint MyJNI_CreateJavaVM(char *PATHDLL,JavaVM **pvm, void **penv, void *args)
{
	char *JVMDLLFULLNAME=NULL;
	JNI_CreateJavaVMPROC ptr_JNI_CreateJavaVM;

	JVMDLLFULLNAME=(char*)malloc( (strlen(PATHDLL)+strlen("/jvm.dll")+1)*sizeof(char));
	sprintf(JVMDLLFULLNAME,"%s/jvm.dll",PATHDLL);

	hLibJVM = LoadLibrary(JVMDLLFULLNAME);
    ptr_JNI_CreateJavaVM = (JNI_CreateJavaVMPROC) GetProcAddress(hLibJVM, "JNI_CreateJavaVM" ); 
	if (JVMDLLFULLNAME){free(JVMDLLFULLNAME);JVMDLLFULLNAME=NULL;};

	return (*ptr_JNI_CreateJavaVM)(pvm,penv,args);
}
#endif
/*-----------------------------------------------------------------------------------*/ 
 int StartJVM(char *SCILAB_PATH)
{
	#define JRE_PATH "/Java/jre1.5.0_06"
	int bOK=FALSE;
	long status;
	char *JVMDLLPATH=NULL;
	char *JAVACLASSPATH=NULL;
	char *JAVALIBRARYPATH=NULL;

	JavaVMInitArgs vm_args;
	JavaVMOption options[3];

	JVMDLLPATH=(char*)malloc( (strlen(SCILAB_PATH)+strlen(JRE_PATH)+strlen("/bin/client")+1)*sizeof(char) );
	sprintf(JVMDLLPATH,"%s%s%s",SCILAB_PATH,JRE_PATH,"/bin/client");

	JAVACLASSPATH=(char*)malloc( (strlen("-Djava.class.path=%s/bin;.")+strlen(SCILAB_PATH)+1)*sizeof(char) );
	sprintf(JAVACLASSPATH,"-Djava.class.path=%s/bin;.",SCILAB_PATH);

	JAVALIBRARYPATH=(char*)malloc( (strlen("-Djava.library.path=%s/lib")+strlen(JRE_PATH)+1)*sizeof(char) );
	sprintf(JAVALIBRARYPATH,"-Djava.library.path=%s/lib",JRE_PATH);

	options[0].optionString = "-Djava.compiler=NONE"; /* disable JIT */
	options[1].optionString = JAVACLASSPATH; /* user classes */
	options[2].optionString = JAVALIBRARYPATH;  /* set native library path */

	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = 3;
	vm_args.ignoreUnrecognized = TRUE;

	#if _MSC_VER
	status = MyJNI_CreateJavaVM(JVMDLLPATH,&jvm, (void**) &env, &vm_args);
	#else
	status = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
	#endif
	
	if (status == JNI_ERR)
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}

	if (JVMDLLPATH) {free(JVMDLLPATH);JVMDLLPATH=NULL;};
	if (JAVACLASSPATH) {free(JAVACLASSPATH);JAVACLASSPATH=NULL;};
	if (JAVALIBRARYPATH) {free(JAVALIBRARYPATH);JAVALIBRARYPATH=NULL;};

	//JVMCallStaticVoidMethod("Prog","Main");

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
void JVMCallStaticVoidMethod(char *Class,char *Method)
{
	jclass cls;
	jmethodID mid;

	cls = (*env)->FindClass(env, Class);
	if (cls == NULL) 
	{
		fprintf(stderr, "Can't find %s class\n",Class);
	}
	else
	{
		mid = (*env)->GetStaticMethodID(env, cls, Method, "([Ljava/lang/String;)V");
		if (mid == NULL) 
		{
			fprintf(stderr, "Can't find %s.%s\n",Class,Method);
		}
		else (*env)->CallStaticVoidMethod(env, cls, mid, NULL);
	}
}
/*-----------------------------------------------------------------------------------*/ 
