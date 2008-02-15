/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_STRING__
#define __GW_STRING__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_string)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_part) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_string) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_convstr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_emptystr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_str2code) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_code2str) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsubst) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ascii) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tokens) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_stripblanks) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcmp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isletter) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isdigit) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isalphanum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isascii) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strcspn) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strncpy) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strrchr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strchr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strstr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strrev) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strtok) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strspn) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_strtod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_regexp) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_STRING__ */
/*--------------------------------------------------------------------------*/
