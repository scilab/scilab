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

#ifndef __GW_BOOLEAN__
#define __GW_BOOLEAN__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int gw_boolean(void);
/*--------------------------------------------------------------------------*/
int C2F(intor)(char *fname,unsigned long fname_len);
int C2F(intand)(char *fname,unsigned long fname_len);
int C2F(sci_find)(char *fname,unsigned long fname_len);
int C2F(sci_bool2s)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_BOOLEAN__ */
/*--------------------------------------------------------------------------*/
