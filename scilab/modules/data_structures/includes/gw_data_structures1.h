/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_DATA_STRUCTURES1__
#define __GW_DATA_STRUCTURES1__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
/* it can be called by fortran mklist.f & mkindx.f */
int C2F(gwdatastructures1)(void); 
int gw_data_structures1(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_glist)(char *fname,unsigned long fname_len);
int C2F(sci_scilist)(char *fname,unsigned long fname_len);
int C2F(sci_lsslist)(char *fname,unsigned long fname_len);
int C2F(sci_rlist)(char *fname,unsigned long fname_len);
int C2F(sci_scitlist)(char *fname,unsigned long fname_len);
int C2F(sci_lstcat)(char *fname,unsigned long fname_len);
int C2F(sci_scimlist)(char *fname,unsigned long fname_len);
int C2F(sci_definedfields)(char *fname,unsigned long fname_len);
int C2F(sci_lstsize)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_DATA_STRUCTURES1__ */
/*--------------------------------------------------------------------------*/

