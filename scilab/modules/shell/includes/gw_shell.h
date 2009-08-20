/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_SHELL_H__
#define __GW_SHELL_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_shell(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_clc)(char *fname,unsigned long fname_len);
int C2F(sci_tohome)(char *fname,unsigned long fname_len);
int C2F(sci_lines)(char *fname,unsigned long fname_len);
int C2F(sci_prompt)(char *fname,unsigned long fname_len);
int sci_iswaitingforinput(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SHELL_H__ */


