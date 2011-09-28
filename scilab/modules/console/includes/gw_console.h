/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_CONSOLE_H__
#define __GW_CONSOLE_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
CONSOLE_IMPEXP int gw_console(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
CONSOLE_IMPEXP int sci_clc(char *fname,unsigned long fname_len);
CONSOLE_IMPEXP int sci_tohome(char *fname,unsigned long fname_len);
CONSOLE_IMPEXP int sci_lines(char *fname,unsigned long fname_len);
CONSOLE_IMPEXP int sci_prompt(char *fname,unsigned long fname_len);
CONSOLE_IMPEXP int sci_iswaitingforinput(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CONSOLE_H__ */


