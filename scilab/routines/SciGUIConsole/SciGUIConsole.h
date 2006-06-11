/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIGUICONSOLE__
#define __SCIGUICONSOLE__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include "jni.h"

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

#ifdef _MSC_VER
	#ifdef EXPORT_SCIGUICONSOLE_DLL
		#define IMPORT_EXPORT_SCIGUICONSOLE_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_SCIGUICONSOLE_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_SCIGUICONSOLE_DLL   extern
#endif

#ifdef _MSC_VER
	#define IMPORT_DLL __declspec(dllimport)
#else
	#define IMPORT_DLL   extern
#endif

#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

IMPORT_EXPORT_SCIGUICONSOLE_DLL int JVM_Create_SciGUIConsole_Object(JNIEnv *env);
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Initialize_SciGUIConsole_Object(JNIEnv *env);
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Events_Loop_SciGUIConsole(JNIEnv *env);
IMPORT_EXPORT_SCIGUICONSOLE_DLL int IsEnabled_SciGUIConsole(JNIEnv *env);
IMPORT_EXPORT_SCIGUICONSOLE_DLL int Dispose_SciGUIConsole(JNIEnv *env);
IMPORT_EXPORT_SCIGUICONSOLE_DLL jobject Get_SciGUIConsole_Object(void);
IMPORT_EXPORT_SCIGUICONSOLE_DLL int PutString(char *Str);


#endif /* __SCIGUICONSOLE__ */
/*-----------------------------------------------------------------------------------*/ 
