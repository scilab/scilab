#include "matscicos.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if WIN32
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int inttimescicos _PARAMS((char *fname,unsigned long l));
extern int intduplicate _PARAMS((char *fname,unsigned long l));
extern int intdiffobjs _PARAMS((char *fname,unsigned long l));
extern int intxproperty _PARAMS((char *fname,unsigned long l));
extern int intphasesim _PARAMS((char *fname,unsigned long l));
extern int intsetxproperty _PARAMS((char *fname,unsigned long l));
extern int intcpass2 _PARAMS((char *fname,unsigned long l));
extern int intsetblockerror _PARAMS((char *fname,unsigned long l));
extern int inttree2 _PARAMS((char *fname,unsigned long l));
extern int inttree3 _PARAMS((char *fname,unsigned long l));
extern int inttree4 _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/ 
static intcscicosTable Tab[]={
  {inttimescicos,"scicos_time"},
  {intduplicate,"duplicate"},
  {intdiffobjs,"diffobjs"},
  {intxproperty,"pointer_xproperty"},
  {intphasesim,"phase_simulation"},
  {intsetxproperty,"set_xproperty"},
  {intcpass2,"scicos_cpass2"},
  {intsetblockerror,"set_blockerror"},
  {inttree2,"ctree2"},
  {inttree3,"ctree3"},
  {inttree4,"ctree4"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(intcscicos)()
{  
	Rhs = Max(0, Rhs);
	#if WIN32
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
