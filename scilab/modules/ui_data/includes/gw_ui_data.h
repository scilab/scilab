/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_UI_DATA__
#define __GW_UI_DATA__
/*--------------------------------------------------------------------------*/
#include "dynlib_ui_data.h"
/*--------------------------------------------------------------------------*/
UI_DATA_IMPEXP int gw_ui_data(void);
UI_DATA_IMPEXP int sci_editvar(char* fname, unsigned long l);
UI_DATA_IMPEXP int sci_browsevar(char* fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /* __GW_UI_DATA__ */
/*--------------------------------------------------------------------------*/
