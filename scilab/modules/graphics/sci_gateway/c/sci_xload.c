/*------------------------------------------------------------------------*/
/* file: sci_xload.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xload routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xload.h"
#include "stack-c.h"
#include "GetProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xload(char *fname,unsigned long fname_len)
{
  CheckRhs(1,2);
 
  /* call a Scilab function to handle compatibility */
  C2F(callscifun)("xload",(unsigned long) 5);
  return 0;
}

/*--------------------------------------------------------------------------*/
