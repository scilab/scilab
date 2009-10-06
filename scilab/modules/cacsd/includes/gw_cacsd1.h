
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
#ifndef __GW_CACSD1__
#define __GW_CACSD1__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int gw_cacsd1(void);
/*--------------------------------------------------------------------------*/
int C2F(scilyap)(char *fname,unsigned long fname_len);
int C2F(scisylv)(char *fname,unsigned long fname_len);
int C2F(intricc)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CACSD1__ */
/*--------------------------------------------------------------------------*/

