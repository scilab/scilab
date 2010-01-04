/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GW_TIME_H__
#define __GW_TIME_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_time.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
TIME_IMPEXP int gw_time(void);
/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
/*--------------------------------------------------------------------------*/ 
TIME_IMPEXP int sci_getdate(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_calendar(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_timer(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_sleep(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_xpause(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_realtimeinit(char *fname,unsigned long fname_len);
TIME_IMPEXP int sci_realtime(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_TIME_H__ */
/*--------------------------------------------------------------------------*/
