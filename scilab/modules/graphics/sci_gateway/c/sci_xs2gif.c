/*------------------------------------------------------------------------*/
/* file: sci_xs2gif.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2gif routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2gif.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2gif( char * fname, unsigned long fname_len )
{
  return xs2file( fname, "GIF", fname_len, 3 ) ;
}
/*--------------------------------------------------------------------------*/
