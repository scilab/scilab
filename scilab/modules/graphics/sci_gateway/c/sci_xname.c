/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_xname.c                                                      */
/* desc : interface for xname routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xname.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_xname(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  
  sciSetName( sciGetCurrentFigure(), cstk(l1), m1 ) ;

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
