/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "jvms.h"
/*-----------------------------------------------------------------------------------*/ 
extern int StartJVM_CONSOLE(char *SCILAB_PATH);
extern int StartJVM_SCILAB(char *SCILAB_PATH);
extern int StartJVM_GRAPHIC(char *SCILAB_PATH);
extern int TerminateJVM_CONSOLE(void);
extern int TerminateJVM_SCILAB(void);
extern int TerminateJVM_GRAPHIC(void);
#if _MSC_VER
extern int GetJVMDll(char *SCILAB_PATH);
extern int FreeJVMDll(void);
#endif

IMPORT_DLL int C2F(xscion)(int *i);
/*-----------------------------------------------------------------------------------*/ 
 IMPORT_EXPORT_LIBJVM_DLL int StartJVMs(char *SCILAB_PATH)
{
	int bOK=FALSE;
	int b1=FALSE,b2=FALSE,b3=FALSE;
	int WithGUIConsole=FALSE;
	
	#if _MSC_VER
	GetJVMDll(SCILAB_PATH);
	#endif

	
	C2F(xscion)(&WithGUIConsole);
	if (WithGUIConsole == TRUE) 
    {
      b1=StartJVM_CONSOLE(SCILAB_PATH);
    }
  	/*b2=StartJVM_SCILAB(SCILAB_PATH);
	b3=StartJVM_GRAPHIC(SCILAB_PATH);
	
	if (b1 && b2 && b3) bOK=TRUE;*/
		
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LIBJVM_DLL int TerminateJVMs(void)
{
	int bOK=FALSE;
	int b1=FALSE,b2=FALSE,b3=FALSE;
	int WithGUIConsole=FALSE;
	
	C2F(xscion)(&WithGUIConsole);
	if (WithGUIConsole == TRUE) 
    {
		b1=TerminateJVM_CONSOLE();
	}

	b1=TerminateJVM_CONSOLE();
	/*b2=TerminateJVM_SCILAB();
	b3=TerminateJVM_GRAPHIC();*/
	
	#if _MSC_VER
	FreeJVMDll();
	#endif
	
	/*if (b1 && b2 && b3) bOK=TRUE;*/
		
	return bOK;

}
/*-----------------------------------------------------------------------------------*/ 
