#include "intwintools.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if WIN32
  #include <Windows.h>
  #include "../os_specific/win_mem_alloc.h"
  extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#if defined (linux)
#include <string.h>
#endif

/*-----------------------------------------------------------------------------------*/
extern int C2F(intwinopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(intwinqueryreg) _PARAMS((char *fname,unsigned long l));
extern int C2F(intgetlongpathname) _PARAMS((char *fname,unsigned long l));
extern int C2F(intgetshortpathname) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddeopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddeclose) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddeexec) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddepoke) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddereq) _PARAMS((char *fname,unsigned long l));
extern int C2F(intddeisopen) _PARAMS((char *fname,unsigned long l));
extern int C2F(intclipboard) _PARAMS((char *fname,unsigned long l));
extern int C2F(inthidetoolbar) _PARAMS((char *fname,unsigned long l));
extern int C2F(inttoolbar) _PARAMS((char *fname,unsigned long l));
extern int C2F(intsetlanguagemenu) _PARAMS((char *fname,unsigned long l));
extern int C2F(intconsoledos) _PARAMS((char *fname,unsigned long l));
extern int C2F(intabout) _PARAMS((char *fname,unsigned long l));
extern int C2F(intmcisendstring) _PARAMS((char *fname,unsigned long l));
extern int C2F(intoemtochar) _PARAMS((char *fname,unsigned long l));
extern int C2F(intchartooem) _PARAMS((char *fname,unsigned long l));
extern int C2F(intprintsetupbox) _PARAMS((char *fname,unsigned long l));
extern int C2F(inttoprint) _PARAMS((char *fname,unsigned long l));
extern int C2F(intsettextcolor) _PARAMS((char *fname,unsigned long l));
extern int C2F(intsettextbackgroundcolor) _PARAMS((char *fname,unsigned long l));
extern int C2F(intfilesassociationbox) _PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/
 static WintoolsTable Tab[]=
 {
  {C2F(intwinopen),"winopen"},
  {C2F(intwinqueryreg),"winqueryreg"},
  {C2F(intgetlongpathname),"getlongpathname"},
  {C2F(intgetshortpathname),"getshortpathname"},
  {C2F(intddeopen),"ddeopen"},
  {C2F(intddeclose),"ddeclose"},
  {C2F(intddeexec),"ddeexec"},
  {C2F(intddepoke),"ddepoke"},
  {C2F(intddereq),"ddereq"},
  {C2F(intddeisopen),"ddeisopen"},
  {C2F(intclipboard),"ClipBoard"},
  {C2F(inthidetoolbar),"hidetoolbar"},
  {C2F(inttoolbar),"toolbar"},
  {C2F(intsetlanguagemenu),"setlanguagemenu"},
  {C2F(intconsoledos),"console"},
  {C2F(intabout),"about"},
  {C2F(intmcisendstring),"mcisendstring"},
  {C2F(intoemtochar),"oemtochar"},
  {C2F(intchartooem),"chartooem"},
  {C2F(intprintsetupbox),"printsetupbox"},
  {C2F(inttoprint),"toprint"},
  {C2F(intsettextcolor),"settextcolor"},
  {C2F(intsettextbackgroundcolor),"settextbackgroundcolor"},
  {C2F(intfilesassociationbox),"filesassociationbox"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(intwintools)()
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

  return 0;
}
/*-----------------------------------------------------------------------------------*/
