
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
#include "api_scilab.h"
#include "MALLOC.h"
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
  {NULL, ""}, //int32
  {NULL, ""}, //int16
  {NULL, ""}, //int8
  {NULL, ""}, //inttype
  {NULL, ""}, //double
  {NULL, ""}, //uint32
  {NULL, ""}, //uint16
  {NULL, ""}, //uint8
  {C2F(scimgeti),"mgeti"},
  {C2F(scivoid),"sci_void"},
  {C2F(sciimput),"%i_mput"},
  {C2F(sciiabs),"%i_abs"},
  {C2F(sciidiag),"%i_diag"},
  {C2F(sciitriu),"%i_triu"},
  {C2F(sciitril),"%i_tril"},
  {C2F(sciisum),"%i_sum"},
  {C2F(sciicumsum),"%i_cumsum"},
  {C2F(sciimax),"%i_max"},
  {C2F(sciimin),"%i_min"},
  {C2F(sciip),"%i_p"},
  {C2F(sciimatrix),"%i_matrix"},
  {C2F(sciiprod),"%i_prod"},
  {C2F(sciicumprod),"%i_cumprod"}
};
/*--------------------------------------------------------------------------*/
int gw_integer(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
