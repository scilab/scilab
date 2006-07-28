#include "gw_fileio.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET (Reorganisation)*/
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
	#include <Windows.h>
	#include "MALLOC.h"
	extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int int_objprintf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objfprintf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objsprintf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objscanf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objfscanf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objsscanf __PARAMS((char *fname,unsigned long fname_len));
extern int int_objfscanfMat __PARAMS((char *fname,unsigned long fname_len));
extern int int_objnumTokens __PARAMS((char *fname,unsigned long fname_len));
extern int int_objfprintfMat __PARAMS((char *fname,unsigned long fname_len));
extern int intsmclearerr __PARAMS((char *fname,unsigned long fname_len));
extern int intsmeof __PARAMS((char *fname,unsigned long fname_len));
extern int intsmopen __PARAMS((char *fname,unsigned long fname_len));
extern int intsmputstr __PARAMS((char *fname,unsigned long fname_len));
extern int intsmclose __PARAMS((char *fname,unsigned long fname_len));
extern int intsmerror __PARAMS((char *fname,unsigned long fname_len));
extern int intsmputstr __PARAMS((char *fname,unsigned long fname_len));
extern int intsmput __PARAMS((char *fname,unsigned long fname_len));
extern int intsmget __PARAMS((char *fname,unsigned long fname_len));
extern int intsmgetstr __PARAMS((char *fname,unsigned long fname_len));
extern int intsmseek __PARAMS((char *fname,unsigned long fname_len));
extern int intsmtell __PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_xls_open) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_xls_read) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
/*  interface function */
/*-----------------------------------------------------------------------------------*/ 
static TabF Tab[]={
	{ intsmopen, "mopen"},
	{ intsmputstr, "mputstr"},
	{ intsmclose, "mclose"},
	{ intsmput, "mput"},
	{ intsmget, "mget"},
	{ intsmgetstr, "mgetstr"},
	{ intsmeof, "meof"},
	{ intsmseek, "mseek"},
	{ intsmtell, "mtell"},
	{ intsmclearerr, "mclearerr"},
	{int_objprintf,"mprintf"},
	{int_objfprintf,"mfprintf"},
	{int_objsprintf,"msprintf"},
	{int_objscanf,"mscanf"},
	{int_objfscanf,"mfscanf"},
	{int_objsscanf,"msscanf"},
	{int_objfscanfMat,"fscanfMat"},
	{int_objfprintfMat,"fprintfMat"},
	{int_objnumTokens,"NumTokens"},
	{intsmerror, "merror"},
	{C2F(sci_xls_open),"xls_open"},
	{C2F(sci_xls_read),"xls_read"}
	};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_fileio)(void)
{
	Rhs=Max(0,Rhs);
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

