#include "gw_io.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_oldload) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_read) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_exec) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_lib) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_diary) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_oldsave) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_write) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_print) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_deff) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_rat) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_file) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_host) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_unix) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_readb) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_writb) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_execstr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_disp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getpid) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getenv) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_read4b) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_write4b) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_save) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_load) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_mgetl) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fileinfo) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getio) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_setenv) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intexec)();
extern int C2F(intexecstr)();
extern int C2F(intgetf)();
extern int C2F(intsave)();
extern int C2F(intload)();
extern int C2F(intdisp)();
extern int C2F(intexec)();
/*-----------------------------------------------------------------------------------*/
static IoTable Tab[]=
{
{C2F(sci_oldload),"oldload"},
{C2F(sci_read),"read"},
{C2F(sci_getf),"getf"},
{C2F(sci_exec),"exec"},
{C2F(sci_lib),"lib"},
{C2F(sci_diary),"diary"},
{C2F(sci_oldsave),"oldsave"},
{C2F(sci_write),"write"},
{C2F(sci_print),"print"},
{C2F(sci_rat),"rat"},
{C2F(sci_deff),"deff"},
{C2F(sci_file),"file"},
{C2F(sci_host),"host"},
{C2F(sci_unix),"unix"},
{C2F(sci_readb),"readb"},
{C2F(sci_writb),"writb"},
{C2F(sci_execstr),"execstr"},
{C2F(sci_disp),"disp"},
{C2F(sci_getpid),"getpid"},
{C2F(sci_getenv),"getenv"},
{C2F(sci_read4b),"read4b"},
{C2F(sci_write4b),"write4b"},
{C2F(sci_save),"save"},
{C2F(sci_load),"load"},
{C2F(sci_mgetl),"mgetl"},
{C2F(sci_fileinfo),"fileinfo"},
{C2F(sci_getio),"getio"},
{C2F(sci_setenv),"setenv"},
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_io)()
{  
	 /* Recursion */
	 if (C2F(recu).rstk[C2F(recu).pt-1] / 100 == 9)
	 {
		switch ((int)(C2F(recu).rstk[C2F(recu).pt-1] - 901))
		{
			case 1: 
			{
				C2F(intexec)();
				return 0;
			}
			case 2: 
			{
				C2F(intexecstr)();
				return 0;
			}
			case 3: 
			{
				C2F(intgetf)();
				return 0;
			}
			case 4:  
			{
				C2F(intsave)();
				return 0;
			}
			case 5:
			{
				C2F(intload)();
			}
			case 6: 
			{
				return 0;
			}
			case 7:
			{
				C2F(intdisp)();
				return 0;
			}
			case 8: 
			{
				C2F(intexec)();
				return 0;
			}
		}
	}
	 else
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
	 }
	return 0;
}
/*-----------------------------------------------------------------------------------*/
