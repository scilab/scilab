#include "gw_integer.h"
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
extern int C2F(sciint32) _PARAMS((char *fname));
extern int C2F(sciint16) _PARAMS((char *fname));
extern int C2F(sciint8) _PARAMS((char *fname));
extern int C2F(sciinttype) _PARAMS((char *fname));
extern int C2F(scidouble) _PARAMS((char *fname));
extern int C2F(sciuint32) _PARAMS((char *fname));
extern int C2F(sciuint16) _PARAMS((char *fname));
extern int C2F(sciuint8) _PARAMS((char *fname));
extern int C2F(scimgeti) _PARAMS((char *fname));
extern int C2F(sciimput) _PARAMS((char *fname));
extern int C2F(sciiconvert) _PARAMS((char *fname));
extern int C2F(sciiabs) _PARAMS((char *fname));
extern int C2F(sciidiag) _PARAMS((char *fname));
extern int C2F(sciitriu) _PARAMS((char *fname));
extern int C2F(sciitril) _PARAMS((char *fname));
extern int C2F(sciisum) _PARAMS((char *fname));
extern int C2F(sciicumsum) _PARAMS((char *fname));
extern int C2F(sciimax) _PARAMS((char *fname));
extern int C2F(sciimin) _PARAMS((char *fname));
extern int C2F(sciimaxi) _PARAMS((char *fname));
extern int C2F(sciimini) _PARAMS((char *fname));
extern int C2F(sciip) _PARAMS((char *fname));
extern int C2F(sciiconvert) _PARAMS((char *fname));
extern int C2F(sciimatrix) _PARAMS((char *fname));
extern int C2F(sciiprod) _PARAMS((char *fname));
extern int C2F(sciicumprod) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/	
static int C2F(scivoid) _PARAMS((char *fname))
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static IntegerTable Tab[]=
{
  {C2F(sciint32),"int32"},
  {C2F(sciint16),"int16"},
  {C2F(sciint8),"int8"},
  {C2F(sciinttype),"inttype"},
  {C2F(scidouble),"double"},
  {C2F(sciuint32),"uint32"},
  {C2F(sciuint16),"uint16"},
  {C2F(sciuint8),"uint8"},
  {C2F(scimgeti),"mgeti"},
  {C2F(scivoid),"sci_void"},
  {C2F(sciimput),"%i_mput"},
  {C2F(sciiconvert),"iconvert"},
  {C2F(sciiabs),"%i_abs"},
  {C2F(sciidiag),"%i_diag"},
  {C2F(sciitriu),"%i_triu"},
  {C2F(sciitril),"%i_tril"},
  {C2F(sciisum),"%i_sum"},
  {C2F(sciicumsum),"%i_cumsum"},
  {C2F(sciimax),"%i_max"},
  {C2F(sciimin),"%i_min"},
  {C2F(sciip),"%i_p"},
  {C2F(sciiconvert),"iconvert"},
  {C2F(sciimatrix),"%i_matrix"},
  {C2F(sciiprod),"%i_prod"},
  {C2F(sciicumprod),"%i_cumprod"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_integer)()
{  
#if _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
