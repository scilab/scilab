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

//YaSp
TIME_IMPEXP int sci_xpause(char *fname, int* _piKey);
TIME_IMPEXP int sci_sleep(char *fname, int* _piKey);
TIME_IMPEXP int sci_realtimeinit(char *fname, int* _piKey);
TIME_IMPEXP int sci_realtime(char *fname, int* _piKey);
TIME_IMPEXP int sci_getdate(char *fname, int* _piKey);
TIME_IMPEXP int sci_calendar(char *fname, int* _piKey);
TIME_IMPEXP int sci_timer(char *fname,int* _piKey);

/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_TIME_H__ */
/*--------------------------------------------------------------------------*/
