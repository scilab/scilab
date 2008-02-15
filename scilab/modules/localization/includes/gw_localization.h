/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET 
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_LOCALIZATION__
#define __GW_LOCALIZATION__
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_localization)(void);
/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
int C2F(sci_setlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getdefaultlanguage) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gettext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_dgettext) _PARAMS((char *fname,unsigned long fname_len));
/*int C2F(sci_getlanguagealias) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlanguagestable) _PARAMS((char *fname,unsigned long fname_len));

int C2F(sci_addtext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_removetext) _PARAMS((char *fname,unsigned long fname_len));*/
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_LOCALIZATION__ */
/*--------------------------------------------------------------------------*/


