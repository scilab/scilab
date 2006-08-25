#include "gw_elementaries_functions.h"
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
extern int C2F(sci_abs) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_real) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_imag) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_conj) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_round) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_int) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_size) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sum) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_prod) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_diag) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_triu) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_tril) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_eye) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_rand) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ones) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_maxi) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sort) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_kron) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_scimatrix) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sin) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_cos) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_atan) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_exp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sqrt) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_log) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_chinesehat) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sign) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_clean) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_floor) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ceil) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_expm) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_cumsum) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_cumprod) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_testmatrix) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_isreal) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_frexp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_zeros) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_tan) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_log1p) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_imult) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_asin) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_acos) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_number_properties) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_nearfloat) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_dsearch) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_isequal) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_spones) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static ElementariesFunctionsTable Tab[]=
{
	{C2F(sci_abs),"abs"},
	{C2F(sci_real),"real"},
	{C2F(sci_imag),"imag"},
	{C2F(sci_conj),"conj"},
	{C2F(sci_round),"round"},
	{C2F(sci_int),"int"},
	{C2F(sci_size),"size"},
	{C2F(sci_sum),"sum"},
	{C2F(sci_prod),"prod"},
	{C2F(sci_diag),"diag"},
	{C2F(sci_triu),"triu"},
	{C2F(sci_tril),"tril"},
	{C2F(sci_eye),"eye"},
	{C2F(sci_rand),"rand"},
	{C2F(sci_ones),"ones"},
	{C2F(sci_maxi),"maxi"},
	{C2F(sci_maxi),"mini"},
	{C2F(sci_sort),"sort"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_kron),"kron"},
	{C2F(sci_scimatrix),"matrix"},	
	{C2F(sci_sin),"sin"},
	{C2F(sci_cos),"cos"},
	{C2F(sci_atan),"atan"},
	{C2F(sci_exp),"exp"},
	{C2F(sci_sqrt),"sqrt"},
	{C2F(sci_log),"log"},
	{C2F(sci_chinesehat),"^"},
	{C2F(sci_sign),"sign"},
	{C2F(sci_clean),"clean"},
	{C2F(sci_floor),"floor"},
	{C2F(sci_ceil),"ceil"},
	{C2F(sci_expm),"expm"},
	{C2F(sci_cumsum),"cumsum"},
	{C2F(sci_cumprod),"cumprod"},
	{C2F(sci_testmatrix),"testmatrix"},
	{C2F(sci_isreal),"isreal"},
	{C2F(sci_frexp),"frexp"},
	{C2F(sci_zeros),"zeros"},
	{C2F(sci_tan),"tan"},
	{C2F(sci_log1p),"log1p"},
	{C2F(sci_imult),"imult"},
	{C2F(sci_asin),"asin"},
	{C2F(sci_acos),"acos"},
	{C2F(sci_number_properties),"number_properties"},
	{C2F(sci_nearfloat),"nearfloat"},
	{C2F(sci_dsearch),"dsearch"},
	{C2F(sci_isequal),"isequal"},
	{C2F(sci_spones),"spones"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_elementaries_functions)()
{  
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
