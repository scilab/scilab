/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_DIFFERENTIAL_EQUATIONS3__
#define __GW_DIFFERENTIAL_EQUATIONS3__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_differential_equations3(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_odc)(char *fname,unsigned long l);
/*--------------------------------------------------------------------------*/
#endif  /* __GW_DIFFERENTIAL_EQUATIONS3__ */
/*--------------------------------------------------------------------------*/
