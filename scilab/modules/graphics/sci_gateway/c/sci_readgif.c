/*------------------------------------------------------------------------*/
/* file: sci_readgif.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for sci_readgif routine                               */
/*------------------------------------------------------------------------*/

#include "sci_readgif.h"
#include "MALLOC.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/ 
int sci_readgif ( char * fname, unsigned long fname_len )
{
  CheckLhs(2,2);
  CheckRhs(1,1);

  /* TODO */

  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
