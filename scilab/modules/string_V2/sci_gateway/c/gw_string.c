#include "gw_string.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(gw_string)(void);
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
    {C2F(sci_length1),"length1"},
	{C2F(sci_regexp),"regexp"},
	{C2F(sci_strindex1),"strindex1"},
    {C2F(sci_strsubst1),"strsubst1"},
	{C2F(sci_grep1),"grep1"},
    {C2F(sci_regexppri),"regexppri"},
	{C2F(sci_emptystr1),"emptystr1"},
    {C2F(sci_strcat1),"strcat1"},
    {C2F(sci_part1),"part1"},
	{C2F(sci_convstr1),"convstr1"},
	{C2F(sci_ascii1),"ascii1"},
    {C2F(sci_stripblanks1),"stripblanks1"},
	{C2F(sci_strsplit),"strsplit"},
	{C2F(sci_tokens1),"tokens1"},
	{C2F(sci_code2str1),"code2str1"}





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
