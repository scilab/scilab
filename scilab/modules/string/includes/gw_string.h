/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_STRING__
#define __GW_STRING__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_string)(void);
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_part) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_string) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_convstr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_emptystr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_str2code) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_code2str) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ssort) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsubst) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ascii) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tokens) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
#endif /*  __GW_STRING__ */
/*-----------------------------------------------------------------------------------*/
