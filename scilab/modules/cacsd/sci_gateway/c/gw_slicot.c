
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
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
#ifndef __DEF_MXARRAY__
#define __DEF_MXARRAY__
typedef int mxArray;
typedef int Gatefunc (int nlhs,mxArray *plhs[],int nrhs,mxArray *prhs[]);
#endif
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
static GenericTable Tab[]={
  {(Myinterfun) fortran_mex_gateway, C2F(sident),"sident"},
  {(Myinterfun) fortran_mex_gateway, C2F(sorder),"sorder"},
  {(Myinterfun) fortran_mex_gateway, C2F(findbd),"findbd"},
  {(Myinterfun) sci_gateway, intrankqr,"rankqr"},
  {(Myinterfun) sci_gateway, intab01od,"contr"},
  {(Myinterfun) sci_gateway, C2F(intmucomp),"mucomp"},
  {(Myinterfun) sci_gateway, C2F(intricc2),"pet_ricc"},
  {(Myinterfun) sci_gateway, C2F(inthinf),"hinf"},
  {(Myinterfun) sci_gateway, C2F(intdhinf),"dhinf"},
  {(Myinterfun) sci_gateway, C2F(intlinmeq),"linmeq"}
};
/*--------------------------------------------------------------------------*/  
int gw_slicot(void)
{
	Rhs = Max(0, Rhs);

	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;

#ifdef _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))((char*)Tab[Fin-1].name,Tab[Fin-1].F);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
	#endif
	return 0;
}
/*--------------------------------------------------------------------------*/ 

