#include "gw_wintools.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <Windows.h>
  #include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
 static WintoolsTable Tab[]=
 {
  {C2F(sci_winopen),"winopen"},
  {C2F(sci_winqueryreg),"winqueryreg"},
  {C2F(sci_getlongpathname),"getlongpathname"},
  {C2F(sci_getshortpathname),"getshortpathname"},
  {C2F(sci_ddeopen),"ddeopen"},
  {C2F(sci_ddeclose),"ddeclose"},
  {C2F(sci_ddeexec),"ddeexec"},
  {C2F(sci_ddepoke),"ddepoke"},
  {C2F(sci_ddereq),"ddereq"},
  {C2F(sci_ddeisopen),"ddeisopen"},
  {C2F(sci_clipboard),"ClipBoard"},
  {C2F(sci_hidetoolbar),"hidetoolbar"},
  {C2F(sci_toolbar),"toolbar"},
  {C2F(sci_setlanguagemenu),"setlanguagemenu"},
  {C2F(sci_consoledos),"console"},
  {C2F(sci_about),"about"},
  {C2F(sci_mcisendstring),"mcisendstring"},
  {C2F(sci_oemtochar),"oemtochar"},
  {C2F(sci_chartooem),"chartooem"},
  {C2F(sci_printsetupbox),"printsetupbox"},
  {C2F(sci_toprint),"toprint"},
  {C2F(sci_settextcolor),"settextcolor"},
  {C2F(sci_settextbackgroundcolor),"settextbackgroundcolor"},
  {C2F(sci_filesassociationbox),"filesassociationbox"},
	{C2F(sci_dos),"dos"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(gw_wintools)()
{  
  Rhs = Max(0, Rhs);
  #ifdef _MSC_VER
    #ifndef _DEBUG
    _try
    {
  	  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(int)strlen(Tab[Fin-1].name));
    }
    _except (EXCEPTION_EXECUTE_HANDLER)
    {
		ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
    }
    #else
      (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(int)strlen(Tab[Fin-1].name));
    #endif
  #else
    (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(int)strlen(Tab[Fin-1].name));
  #endif

  return 0;
}
/*-----------------------------------------------------------------------------------*/
