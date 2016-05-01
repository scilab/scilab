/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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

/*-----------------------------------------------------------------------------------*/
#include "dynlib_matio.h"
#include "dynlib_matio_gw.h"
/*-----------------------------------------------------------------------------------*/
#ifndef __GW_MATIO_H__
#define __GW_MATIO_H__
/*-----------------------------------------------------------------------------------*/
MATIO_IMPEXP int gw_matio(void);

/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */
/*-----------------------------------------------------------------------------------*/
MATIO_GW_IMPEXP int sci_matfile_open(char* fname, void* pvApiCtx);
MATIO_GW_IMPEXP int sci_matfile_close(char* fname, void* pvApiCtx);
MATIO_GW_IMPEXP int sci_matfile_listvar(char* fname, void* pvApiCtx);
MATIO_GW_IMPEXP int sci_matfile_varreadnext(char* fname, void* pvApiCtx);
MATIO_GW_IMPEXP int sci_matfile_varwrite(char* fname, void* pvApiCtx);
#endif /* __GW_MATIO_H__ */
