#include "gw_cscicos.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
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
extern int intscicosimc _PARAMS((char *fname, unsigned long l));
extern int intgetscicosvarsc _PARAMS((char *fname, unsigned long l));
extern int intcurblkc _PARAMS((char *fname, unsigned long l));
extern int intbuildouttb _PARAMS((char *fname));
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
  {intscicosimc,"scicosim"},
  {intgetscicosvarsc,"getscicosvars"},
  {intcurblkc,"curblockc"},
  {intbuildouttb,"buildouttb"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_cscicos)()
{  
	Rhs = Max(0, Rhs);
	#ifdef _MSC_VER
		#ifndef _DEBUG
			_try
			{
				(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
			}
			_except (EXCEPTION_EXECUTE_HANDLER)
			{
				ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
			}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
	C2F(putlhsvar)();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
