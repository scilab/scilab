/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_string.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*String_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	String_Interf f;    /** function **/
	char *name;      /** its name **/
} StringTable;
/*-----------------------------------------------------------------------------------*/
static StringTable Tab[]=
{
	{C2F(sci_length),"length"},
	{C2F(sci_part),"part"},
	{C2F(sci_string),"string"},
	{C2F(sci_convstr),"convstr"},
	{C2F(sci_emptystr),"emptystr"},
	{C2F(sci_str2code),"str2code"},
	{C2F(sci_code2str),"code2str"},
	{C2F(sci_ssort),"sort"},
	{C2F(sci_strcat),"strcat"},
	{C2F(sci_strindex),"strindex"},
	{C2F(sci_strsubst),"strsubst"},
	{C2F(sci_ascii),"ascii"},
	{C2F(sci_grep),"grep"},
	{C2F(sci_tokens),"tokens"},
	{C2F(sci_strsplit),"strsplit"},
	{C2F(sci_xsort),"gsort"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_string)(void)
{  
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
