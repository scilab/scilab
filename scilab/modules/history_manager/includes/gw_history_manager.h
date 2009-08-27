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
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_history_manager(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_loadhistory)(char *fname,unsigned long fname_len);
int C2F(sci_savehistory)(char *fname,unsigned long fname_len);
int C2F(sci_gethistory)(char *fname,unsigned long fname_len);
int C2F(sci_resethistory)(char *fname,unsigned long fname_len);
int C2F(sci_displayhistory)(char *fname,unsigned long fname_len);
int C2F(sci_addhistory)(char *fname,unsigned long fname_len);
int C2F(sci_saveafterncommands)(char *fname,unsigned long fname_len);
int C2F(sci_saveconsecutivecommands)(char *fname,unsigned long fname_len);
int C2F(sci_gethistoryfile)(char *fname,unsigned long fname_len);
int C2F(sci_sethistoryfile)(char *fname,unsigned long fname_len);
int C2F(sci_removelinehistory)(char *fname,unsigned long fname_len);
int C2F(sci_historysize)(char *fname,unsigned long fname_len);
int C2F(sci_historymanager)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_HISTORY_MANAGER_H__ */
