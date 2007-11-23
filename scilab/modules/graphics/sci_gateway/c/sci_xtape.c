/*------------------------------------------------------------------------*/
/* file: sci_xtape.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xtape routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xtape.h"
#include "Scierror.h"


/*--------------------------------------------------------------------------*/
int sci_xtape( char *fname,unsigned long fname_len )
{
  Scierror(999,"xtape routine was only available with old graphic mode.\n");
  return -1 ;
}
/*--------------------------------------------------------------------------*/
