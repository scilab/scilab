/*------------------------------------------------------------------------*/
/* file: sci_xsave.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xsave routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xsave.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"

/*-----------------------------------------------------------------------------------*/
int sci_xsave( char * fname, unsigned long fname_len )
{

  CheckRhs(1,2);
 
  /* call a scilab macro */
  C2F(callscifun)("xsave",(unsigned long) 6);
  return 0;

}

/*-----------------------------------------------------------------------------------*/
