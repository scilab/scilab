/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#ifndef __GW_DATA_STRUCTURES1__
#define __GW_DATA_STRUCTURES1__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_data_structures.h"
/*--------------------------------------------------------------------------*/
DATA_STRUCTURES_IMPEXP int C2F(gwdatastructures1)(void);
DATA_STRUCTURES_IMPEXP int gw_data_structures1(void);
/*--------------------------------------------------------------------------*/
int sci_scilist(char *fname, unsigned long fname_len);
int sci_scitlist(char *fname, unsigned long fname_len);
int sci_lstcat(char *fname, unsigned long fname_len);
int sci_scimlist(char *fname, unsigned long fname_len);
int sci_lstsize(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_DATA_STRUCTURES1__ */
/*--------------------------------------------------------------------------*/

