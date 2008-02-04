/*------------------------------------------------------------------------*/
/* file: sci_xs2pdf.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Koumar Sylvestre                                             */
/* desc : interface for xs2pdf routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2pdf.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2pdf( char * fname, unsigned long fname_len )
{
  return xs2file( fname, PDF_EXPORT ) ;
}
/*--------------------------------------------------------------------------*/
