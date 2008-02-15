/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_HISTORY_MANAGER_H__
#define __GW_HISTORY_MANAGER_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_history_manager)(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_loadhistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_savehistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gethistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_displayhistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_addhistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_saveafterncommands) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_saveconsecutivecommands) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gethistoryfile) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sethistoryfile) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_removelinehistory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_historysize) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_historymanager) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_HISTORY_MANAGER_H__ */
