/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_ARNOLDI_H__
#define __GW_ARNOLDI_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_arnoldi)(void);
/*--------------------------------------------------------------------------*/
int C2F(intdsaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdnaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intznaupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdseupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intdneupd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intzneupd) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /* __GW_ARNOLDI_H__ */
/*--------------------------------------------------------------------------*/

