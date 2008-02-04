/*------------------------------------------------------------------------*/
/* file: sci_xs2png.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2png routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2png.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2png( char * fname, unsigned long fname_len )
{
  return xs2file( fname, PNG_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
