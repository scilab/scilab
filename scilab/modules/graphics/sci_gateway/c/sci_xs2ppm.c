/*------------------------------------------------------------------------*/
/* file: sci_xs2ppm.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2ppm routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2ppm.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2ppm( char * fname, unsigned long fname_len )
{
  return xs2file( fname, "PPM", fname_len, 3 ) ;
}
/*--------------------------------------------------------------------------*/
