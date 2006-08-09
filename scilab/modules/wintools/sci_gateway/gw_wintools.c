#include "gw_wintools.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
  #include <Windows.h>
  #include "win_mem_alloc.h"
  extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_winqueryreg) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_getlongpathname) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_getshortpathname) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddeopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddeclose) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddeexec) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddepoke) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddereq) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_ddeisopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_clipboard) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_hidetoolbar) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_toolbar) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_setlanguagemenu) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_consoledos) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_about) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_mcisendstring) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_oemtochar) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_chartooem) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_toprint) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_settextcolor) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_settextbackgroundcolor) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_filesassociationbox) _PARAMS((char *fname,unsigned long l));
extern int C2F(sci_dos) _PARAMS((char *fname,unsigned long l));
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
  #if _MSC_VER
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

  return 0;
}
/*-----------------------------------------------------------------------------------*/
