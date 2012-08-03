/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_COMPLETION_H__
#define __GW_COMPLETION_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_completion.h"
/*--------------------------------------------------------------------------*/
COMPLETION_IMPEXP int gw_completion(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
COMPLETION_IMPEXP int sci_completion(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_COMPLETION_H__ */

