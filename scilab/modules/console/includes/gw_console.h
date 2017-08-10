/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_CONSOLE_H__
#define __GW_CONSOLE_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
/*--------------------------------------------------------------------------*/
CONSOLE_IMPEXP int gw_console(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
//CONSOLE_IMPEXP int sci_clc(char *fname,unsigned long fname_len);
//CONSOLE_IMPEXP int sci_tohome(char *fname,unsigned long fname_len);
//CONSOLE_IMPEXP int sci_lines(char *fname, void* pvApiCtx);
//CONSOLE_IMPEXP int sci_prompt(char *fname,unsigned long fname_len);
//CONSOLE_IMPEXP int sci_iswaitingforinput(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CONSOLE_H__ */


