/*------------------------------------------------------------------------*/
/* file: sci_xs2jpg.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2jpg routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2jpg.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2jpg( char * fname, unsigned long fname_len )
{
  return xs2file( fname, JPG_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
