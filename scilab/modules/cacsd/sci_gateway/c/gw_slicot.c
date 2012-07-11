
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

#ifdef _MSC_VER
#pragma warning(disable: 4113)
#endif
/*--------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "machine.h"
#include "../../../mexlib/includes/mex.h"
#include "../../../mexlib/includes/sci_gateway.h"
#include "sci_rankqr.h"
#include "sci_contr.h"
#include "gw_slicot.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
#ifndef __DEF_MXARRAY__
#define __DEF_MXARRAY__
typedef int mxArray;
#endif
typedef int Gatefunc (int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]);
/* fortran subroutines */
extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);
extern Gatefunc C2F(intmucomp);
extern Gatefunc C2F(intricc2);
extern Gatefunc C2F(inthinf);
extern Gatefunc C2F(intdhinf);
extern Gatefunc C2F(intlinmeq);
/*--------------------------------------------------------------------------*/
#ifndef __DEF_TABLE_STRUCT__
#define __DEF_TABLE_STRUCT__

typedef int (*GatefuncH) (int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]);

typedef int (*FGatefuncH) (int *nlhs,mxArray *plhs[],int *nrhs, mxArray *prhs[]);

typedef int (*Myinterfun) (char *, GatefuncH F);

typedef int (*GT) ();

typedef struct table_struct {
    Myinterfun f;    /** interface **/
    GT F;     /** function **/
    char *name;      /** its name **/
  } GenericTable;
#endif

int fortran_mex_gateway (char *fname, FGatefuncH F);

static GenericTable Tab[]={
    {(Myinterfun) NULL, C2F(sident),"sident"}, // was fortran_mex_gateway
  {(Myinterfun) NULL, C2F(sorder),"sorder"}, // was fortran_mex_gateway
  {(Myinterfun) NULL, C2F(findbd),"findbd"}, // was fortran_mex_gateway
  {(Myinterfun) sci_gateway, intrankqr,"rankqr"},
  {(Myinterfun) sci_gateway, intab01od,"contr"},
  {(Myinterfun) sci_gateway, C2F(intmucomp),"mucomp"},
  {(Myinterfun) sci_gateway, C2F(intricc2),"pet_ricc"},
  {(Myinterfun) sci_gateway, C2F(inthinf),"hinf"},
  {(Myinterfun) sci_gateway, C2F(intdhinf),"dhinf"},
  {(Myinterfun) sci_gateway, C2F(intlinmeq),"linmeq"}
};
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
int gw_slicot(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/

