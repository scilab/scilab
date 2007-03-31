/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_cscicos.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
typedef int (*scicosc_interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct {
	scicosc_interf f;    /** function **/
	char *name;      /** its name **/
} intcscicosTable;
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
