
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

#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
int gw_dynamic_link(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/ 
int sci_getdynlibext(char *fname,unsigned long fname_len);
int sci_addinter(char *fname,unsigned long fname_len);
int sci_fort(char *fname,unsigned long fname_len);
int sci_call(char *fname,unsigned long fname_len);
int sci_link(char *fname,unsigned long fname_len);
int sci_ulink(char *fname,unsigned long fname_len);
int sci_c_link(char *fname,unsigned long fname_len);
int sci_ilib_verbose(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_DYNAMIC_LINK__ */
/*--------------------------------------------------------------------------*/ 
