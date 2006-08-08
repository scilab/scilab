#include "gw_string.h"
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
extern int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_part) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_string) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_convstr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_emptystr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_str2code) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_code2str) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ssort) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_strsubst) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ascii) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_tokens) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len));
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
	{C2F(sci_strcat),"strcat "},
	{C2F(sci_strindex),"strindex"},
	{C2F(sci_strsubst),"strsubst"},
	{C2F(sci_ascii),"ascii"},
	{C2F(sci_grep),"grep"},
	{C2F(sci_tokens),"tokens"},
	{C2F(sci_strsplit),"strsplit"}

};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_string)()
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
