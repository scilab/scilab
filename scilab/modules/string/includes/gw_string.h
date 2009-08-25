
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
#include "api_common.h"

/*--------------------------------------------------------------------------*/ 
int gw_string(void);
/*--------------------------------------------------------------------------*/ 
int sci_length(char *fname,unsigned long fname_len);
int sci_part(char *fname,unsigned long fname_len);
int sci_string(char *fname,unsigned long fname_len);
int sci_convstr(char *fname,unsigned long fname_len);
int sci_emptystr(char *fname,unsigned long fname_len);
int sci_str2code(char *fname,unsigned long fname_len);
int sci_code2str(char *fname,unsigned long fname_len);
int sci_strcat(char *fname,unsigned long fname_len);
int sci_strindex(char *fname,unsigned long fname_len);
int sci_strsubst(char *fname,unsigned long fname_len);
int sci_ascii(char *fname,unsigned long fname_len);
int sci_grep(char *fname,unsigned long fname_len);
int sci_tokens(char *fname,unsigned long fname_len);
int sci_strsplit(char *fname,unsigned long fname_len);
int sci_stripblanks(char *fname,unsigned long fname_len);
int sci_strcmp(char *fname,unsigned long fname_len);
int sci_isletter(char *fname,unsigned long fname_len);
int sci_isdigit(char *fname,unsigned long fname_len);
int sci_isalphanum(char *fname,unsigned long fname_len);
int sci_isascii(char *fname,unsigned long fname_len);
int sci_strcspn(char *fname,unsigned long fname_len);
int sci_strncpy(char *fname,unsigned long fname_len);
int sci_strrchr(char *fname,unsigned long fname_len);
int sci_strchr(char *fname,unsigned long fname_len);
int sci_strstr(char *fname,unsigned long fname_len);
int sci_strrev(char *fname,unsigned long fname_len);
int sci_strtok(char *fname,unsigned long fname_len);
int sci_strspn(char *fname,unsigned long fname_len);
int sci_strtod(char *fname,unsigned long fname_len);
int sci_regexp(char *fname,unsigned long fname_len);
int sci_NumTokens(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_STRING__ */
/*--------------------------------------------------------------------------*/
