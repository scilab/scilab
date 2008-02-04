/*------------------------------------------------------------------------*/
/* file: sci_xs2eps.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2eps routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2eps.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2eps( char * fname, unsigned long fname_len )
{
  return xs2file( fname, EPS_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
