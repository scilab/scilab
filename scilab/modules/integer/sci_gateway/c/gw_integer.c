
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
#include <string.h>
#include "gw_integer.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include "BOOL.h"
extern BOOL BuildWithVS8ExpressF2C(void);
#endif
/*--------------------------------------------------------------------------*/
static int C2F(scivoid)(char *fname,unsigned long fname_len)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
  {C2F(sciint32),"int32"},
  {C2F(sciint16),"int16"},
  {C2F(sciint8),"int8"},
  {C2F(sciinttype),"inttype"},
  {C2F(scidouble),"double"},
  {C2F(sciuint32),"uint32"},
  {C2F(sciuint16),"uint16"},
  {C2F(sciuint8),"uint8"},
  {C2F(scimgeti),"mgeti"},
  {C2F(scivoid),"sci_void"},
  {C2F(sciimput),"%i_mput"},
  {C2F(sciiconvert),"iconvert"},
  {C2F(sciiabs),"%i_abs"},
  {C2F(sciidiag),"%i_diag"},
  {C2F(sciitriu),"%i_triu"},
  {C2F(sciitril),"%i_tril"},
  {C2F(sciisum),"%i_sum"},
  {C2F(sciicumsum),"%i_cumsum"},
  {C2F(sciimax),"%i_max"},
  {C2F(sciimin),"%i_min"},
  {C2F(sciip),"%i_p"},
  {C2F(sciiconvert),"iconvert"},
  {C2F(sciimatrix),"%i_matrix"},
  {C2F(sciiprod),"%i_prod"},
  {C2F(sciicumprod),"%i_cumprod"}
};
/*--------------------------------------------------------------------------*/
int gw_integer(void)
{  
	#ifdef _MSC_VER
	#ifndef _DEBUG
	if  ( BuildWithVS8ExpressF2C() )
	{
		/* Bug 4123 F2C code (i_prod.f) returns a wrong exception after callFunctionFromGateway */
		/* and it crashs with release mode */
		/* workaround disabled callFunctionFromGateway and call function without check */
		if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) ((char*)Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	}
	else
	#endif
	#endif
	{
		callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
