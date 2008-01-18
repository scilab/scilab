/*------------------------------------------------------------------------*/
/* file: sci_xs2bmp.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors :  Allan Cornet                                                */
/* desc : interface for xs2bmp routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2bmp.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2bmp( char * fname, unsigned long fname_len )
{
  return xs2file( fname, BMP_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
