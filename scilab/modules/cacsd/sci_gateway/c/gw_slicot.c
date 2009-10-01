
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
#include <math.h>
#include <string.h>
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "../../../mexlib/includes/mex.h"
#include "../../../mexlib/includes/sci_gateway.h"
#include "sci_rankqr.h"
#include "sci_contr.h"
#include "gw_slicot.h"

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

