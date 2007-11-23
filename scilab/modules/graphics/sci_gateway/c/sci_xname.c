/*------------------------------------------------------------------------*/
/* file: sci_xname.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
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
