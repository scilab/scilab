/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_SCINOTES__
#define __GW_SCINOTES__
/*--------------------------------------------------------------------------*/
#include "dynlib_scinotes.h"
/*--------------------------------------------------------------------------*/
SCINOTES_IMPEXP int gw_scinotes(void);
SCINOTES_IMPEXP int sci_scinotes(char* fname, unsigned long l);
SCINOTES_IMPEXP int sci_closeSciNotesFromScilab(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_SCINOTES__ */
/*--------------------------------------------------------------------------*/
