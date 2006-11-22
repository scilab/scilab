#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
typedef int (*Arpack_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Arpack_Interf f;    /** function **/
	char *name;      /** its name **/
} ArpackTable;
/*-----------------------------------------------------------------------------------*/
extern int C2F(intdsaupd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intdnaupd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intznaupd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intdseupd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intdneupd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intzneupd) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_arnoldi)(void);
/*-----------------------------------------------------------------------------------*/
static ArpackTable Tab[]={
  {C2F(intdsaupd),"dsaupd"},
  {C2F(intdnaupd),"dnaupd"},
  {C2F(intznaupd),"znaupd"},
  {C2F(intdseupd),"dseupd"},
  {C2F(intdneupd),"dneupd"},
  {C2F(intzneupd),"zneupd"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_arnoldi)(void)
{
	Rhs = Max(0, Rhs);
	
	#if _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif

	return 0;
}
/*-----------------------------------------------------------------------------------*/


