/*-----------------------------------------------------------------------------------*/ 
#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#include "../../../mexlib/includes/mex.h"
/*-----------------------------------------------------------------------------------*/ 
extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);
extern int intrankqr(char* fname);
extern int intab01od(char* fname);
extern Gatefunc C2F(intmucomp);
extern Gatefunc C2F(intricc2);
extern Gatefunc C2F(inthinf);
extern Gatefunc C2F(intdhinf);
extern Gatefunc C2F(intlinmeq);
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_slicot)(void);
/*-----------------------------------------------------------------------------------*/ 
static GenericTable Tab[]={
  {(Myinterfun) fortran_mex_gateway, C2F(sident),"sident"},
  {(Myinterfun) fortran_mex_gateway, C2F(sorder),"sorder"},
  {(Myinterfun) fortran_mex_gateway, C2F(findbd),"findbd"},
  {(Myinterfun) sci_gateway, intrankqr,"rankqr"},
  {(Myinterfun) sci_gateway, intab01od,"contr"},
  {(Myinterfun) sci_gateway, C2F(intmucomp),"mucomp"},
  {(Myinterfun) sci_gateway, C2F(intricc2),"pet_ricc"},
  {(Myinterfun) sci_gateway, C2F(inthinf),"hinf"},
  {(Myinterfun) sci_gateway, C2F(intdhinf),"dhinf"},
  {(Myinterfun) sci_gateway, C2F(intlinmeq),"linmeq"}
};
/*-----------------------------------------------------------------------------------*/  
int C2F(gw_slicot)(void)
{
	Rhs = Max(0, Rhs);
	#if _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
	#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 

