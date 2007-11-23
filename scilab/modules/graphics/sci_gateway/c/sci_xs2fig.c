/*------------------------------------------------------------------------*/
/* file: sci_xs2fig.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2fig routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2fig.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2fig(char *fname,unsigned long fname_len)
{
  return xs2file( fname, "Fig", fname_len, 3 ) ;
}
/*--------------------------------------------------------------------------*/
