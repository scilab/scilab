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
#ifndef __GW_WINDOWS_TOOLS_H__
#define __GW_WINDOWS_TOOLS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_windows_tools.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
WINDOWS_TOOLS_IMPEXP int gw_windows_tools(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
WINDOWS_TOOLS_IMPEXP int sci_winopen(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_winqueryreg(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_findfileassociation(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_dos(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_mcisendstring(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_oemtochar(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_chartooem(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_consolebox(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_win64(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_istssession(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_getsystemmetrics(char *fname,unsigned long l);
WINDOWS_TOOLS_IMPEXP int sci_createGUID(char *fname,unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /* __GW_WINDOWS_TOOLS_H__ */


