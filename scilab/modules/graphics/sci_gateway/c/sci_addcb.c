/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_addcb.c                                                      */
/* desc : interface for addcb routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_addcb.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "Interaction.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_addcb(char *fname,unsigned long fname_len)
{  
  unsigned long hdl;
  integer m1, n1, l1,m2, n2, l2;
  sciPointObj *pobj;

  CheckRhs(2,3);
  CheckLhs(0,1);

  /*  set or create a graphic window*/
  switch(VarType(1)) 
  {
  case sci_matrix: /* first is a scalar argument so it's a legend(hdl,"str1",...)*/
    CheckRhs(3,3);
    GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1); /* Gets the Handle passed as argument*/
    hdl = (unsigned long)*hstk(l1); /* Retrieve the object pointer through the handle */
    GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1); /* Gets the command name  */
    GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2); /* Gets the mouse event */
    break;
  case sci_strings:/* first is a string argument so it's a sciset("str1",....)*/
    hdl = (unsigned long ) sciGetHandle(sciGetCurrentSubWin()); /* Gets the figure handle value if it ones (the phather)*/
    CheckRhs(2,2);
    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1); /* Gets the command name    */
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2); /* Gets the mouse event */
    break;
  default: 
    Scierror(999,_("%s: Wrong type for input argument #%d: Scalar or string expected.\n"),fname,1);
    return 0;
    break;

  }
  if ((pobj = sciGetPointerFromHandle(hdl)) != NULL )
    sciAddCallback((sciPointObj *)pobj, cstk(l1),m1*n1,*istk(l2));
  else {
    Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
    return 0;
  }
  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
