/*--------------------------------------------------------------------------*/
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
#ifndef __GW_DYNAMIC_LINK__
#define __GW_DYNAMIC_LINK__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_dynamic_link)(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/ 
int C2F(sci_getdynlibext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_addinter) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_link) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ulink) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_c_link) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_DYNAMIC_LINK__ */
/*--------------------------------------------------------------------------*/ 
