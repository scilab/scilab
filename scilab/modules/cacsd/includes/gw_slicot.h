
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
#ifndef __GW_SLICOT_H__
#define __GW_SLICOT_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
#ifndef __DEF_MXARRAY__
#define __DEF_MXARRAY__
typedef int mxArray;
typedef int Gatefunc (int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]);
#endif
/*--------------------------------------------------------------------------*/ 
extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);
extern Gatefunc C2F(intmucomp);
extern Gatefunc C2F(intricc2);
extern Gatefunc C2F(inthinf);
extern Gatefunc C2F(intdhinf);
extern Gatefunc C2F(intlinmeq);
/*--------------------------------------------------------------------------*/ 
int gw_slicot(void);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_SLICOT_H__ */
/*--------------------------------------------------------------------------*/

