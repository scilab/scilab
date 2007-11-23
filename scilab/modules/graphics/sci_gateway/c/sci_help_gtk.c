/*------------------------------------------------------------------------*/
/* file: sci_help_gtk.c                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for help_gtk routine                                  */
/*------------------------------------------------------------------------*/

#include "sci_help_gtk.h"
#include "stack-c.h"

/*--------------------------------------------------------------------------*/
int sci_help_gtk(char *fname,unsigned long fname_len)
{
  /* do nothing */
  CheckLhs(0,1);
  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
