/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#ifndef __GW_TIME_H__
#define __GW_TIME_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_time.h"
#include "dynlib_time_gw.h"
/*--------------------------------------------------------------------------*/
TIME_IMPEXP int gw_time(void);
/*--------------------------------------------------------------------------*/
int sci_xpause(char *fname, void* pvApiCtx);
int sci_sleep(char *fname, void* pvApiCtx);
int sci_realtimeinit(char *fname, void* pvApiCtx);
int sci_realtime(char *fname, void* pvApiCtx);
int sci_getdate(char *fname, void* pvApiCtx);
int sci_calendar(char *fname, void* pvApiCtx);
int sci_timer(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_TIME_H__ */
/*--------------------------------------------------------------------------*/
