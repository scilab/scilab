/*------------------------------------------------------------------------*/
/* file: sci_xs2svg.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2svg routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2svg.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2svg( char * fname, unsigned long fname_len )
{
  return xs2file( fname, SVG_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
