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
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_windows_tools(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l));
int C2F(sci_winqueryreg) _PARAMS((char *fname,unsigned long l));
int C2F(sci_findfileassociation) _PARAMS((char *fname,unsigned long l));
int C2F(sci_dos) _PARAMS((char *fname,unsigned long l));
int C2F(sci_mcisendstring) _PARAMS((char *fname,unsigned long l));
int C2F(sci_oemtochar) _PARAMS((char *fname,unsigned long l));
int C2F(sci_chartooem) _PARAMS((char *fname,unsigned long l));
int C2F(sci_consolebox) _PARAMS((char *fname,unsigned long l));
int C2F(sci_printsetupbox) _PARAMS((char *fname,unsigned long l));
int C2F(sci_toprint) _PARAMS((char *fname,unsigned long l));
/*--------------------------------------------------------------------------*/
#endif /* __GW_WINDOWS_TOOLS_H__ */


