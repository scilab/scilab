
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_SPECIAL_FUNCTIONS__
#define __GW_SPECIAL_FUNCTIONS__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
int gw_special_functions1(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_oldbesseli)(char *fname,unsigned long fname_len);
int C2F(sci_oldbesselj)(char *fname,unsigned long fname_len);
int C2F(sci_oldbesselk)(char *fname,unsigned long fname_len);
int C2F(sci_oldbessely)(char *fname,unsigned long fname_len);
int C2F(sci_gamma)(char *fname,unsigned long fname_len);
int C2F(sci_lgamma)(char *fname,unsigned long fname_len);
int C2F(sci_dlgamma)(char *fname,unsigned long fname_len);
int C2F(sci_calerf)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SPECIAL_FUNCTIONS__ */
/*--------------------------------------------------------------------------*/

