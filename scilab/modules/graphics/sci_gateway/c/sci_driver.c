/*------------------------------------------------------------------------*/
/* file: sci_driver.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for driver routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_driver.h"
#include "stack-c.h"
#include "returnProperty.h"

/*--------------------------------------------------------------------------*/
int sci_driver(char *fname,unsigned long fname_len)
{
  integer m1=3,n1=1,l1;
  CheckRhs(-1,1);
  CheckLhs(0,1);
  if (Rhs <= 0) 
  {
    return sciReturnString("Rec");
  } 
  else 
  {
    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

    LhsVar(1)=0;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
