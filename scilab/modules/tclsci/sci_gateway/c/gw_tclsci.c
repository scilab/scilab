/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
	#include <Windows.h>
	#include "ExceptionMessage.h"
#endif
#include "message_scilab.h"
#include "error_scilab.h"
#include "gw_tclsci.h"
#include "stack-c.h"
#include "inisci-c.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*TCLSci_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	TCLSci_Interf f;    /** function **/
	char *name;      /** its name **/
} TCLSCITable;
/*-----------------------------------------------------------------------------------*/
extern int TK_Started;
/*-----------------------------------------------------------------------------------*/
 static TCLSCITable Tab[]=
 {
  {C2F(sci_TCL_DoOneEvent),"TCL_DoOneEvent"},
  {C2F(sci_TCL_EvalFile),"TCL_EvalFile"},
  {C2F(sci_TCL_EvalStr),"TCL_EvalStr"},
  {C2F(sci_TCL_GetVar),"TCL_GetVar"},
  {C2F(sci_TCL_SetVar),"TCL_SetVar"},
  {C2F(sci_opentk),"opentk"},
  {C2F(sci_close),"close"},
  {C2F(sci_findobj),"findobj"},
  {C2F(sci_TCL_set),"%s_set"},
  {C2F(sci_TCL_get),"%s_get"},
  {C2F(sci_TCL_gcf),"TCL_gcf"},
  {C2F(sci_TCL_scf),"TCL_scf"},
  {C2F(sci_TCL_GetVersion),"TCL_GetVersion"},
  {C2F(sci_TCL_UnsetVar),"TCL_UnsetVar"},
  {C2F(sci_TCL_ExistVar),"TCL_ExistVar"},
  {C2F(sci_TCL_UpVar),"TCL_UpVar"},
  {C2F(sci_TCL_DeleteInterp),"TCL_DeleteInterp"},
  {C2F(sci_TCL_CreateSlave),"TCL_CreateSlave"},
  {C2F(sci_TCL_ExistInterp),"TCL_ExistInterp"},
  {C2F(sci_TCL_ExistArray),"TCL_ExistArray"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(gw_tclsci)()
{  
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (TK_Started)
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
		}
		else
		{
			message_scilab("tclsci_message_1");
		}
	}
	else
	{
		error_scilab(999,"tclsci_error_1");
		return 0;
	}
	
  return 0;
}
/*-----------------------------------------------------------------------------------*/
