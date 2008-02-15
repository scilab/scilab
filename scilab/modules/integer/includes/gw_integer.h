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
#ifndef __GW_INTEGER__
#define __GW_INTEGER__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_integer)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sciint32) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciint16) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciint8) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciinttype) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scidouble) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint32) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint16) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciuint8) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scimgeti) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimput) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiconvert) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiabs) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciidiag) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciitriu) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciitril) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciisum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciicumsum) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimax) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimin) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimaxi) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimini) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciip) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciimatrix) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciiprod) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciicumprod) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_INTEGER__ */
/*--------------------------------------------------------------------------*/

