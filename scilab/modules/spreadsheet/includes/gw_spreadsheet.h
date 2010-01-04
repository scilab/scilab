
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_SPREADSHEET_H__
#define __GW_SPREADSHEET_H__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_spreadsheet.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
SPREADSHEET_IMPEXP int gw_spreadsheet(void);
/*--------------------------------------------------------------------------*/ 
SPREADSHEET_IMPEXP int sci_xls_read(char *fname,unsigned long fname_len);
SPREADSHEET_IMPEXP int sci_xls_open(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SPREADSHEET_H__ */
/*--------------------------------------------------------------------------*/
