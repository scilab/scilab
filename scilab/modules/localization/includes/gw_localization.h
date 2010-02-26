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
#include "dynlib_localization.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
LOCALIZATION_IMPEXP int gw_localization(void);
/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
LOCALIZATION_IMPEXP int sci_setlanguage(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_getlanguage(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_getdefaultlanguage(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_gettext(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_dgettext(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_dgettextchar(char *fname,unsigned long fname_len);
LOCALIZATION_IMPEXP int sci_setdefaultlanguage(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_LOCALIZATION__ */
/*--------------------------------------------------------------------------*/


