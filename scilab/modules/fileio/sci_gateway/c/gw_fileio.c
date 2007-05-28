/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#include <Windows.h>
	#include "ExceptionMessage.h"
#endif
#include "gw_fileio.h"
#include "stack-c.h"
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
	{C2F(sci_xls_read),"xls_read"},
	{C2F(sci_findfiles),"findfiles"},
	{C2F(sci_maxfiles),"maxfiles"},
	{C2F(sci_getdrives),"getdrives"},
	{C2F(sci_fullpath),"fullpath"},
	{C2F(sci_deletefile),"deletefile"}
	};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_fileio)(void)
{
	Rhs=Max(0,Rhs);
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

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 

