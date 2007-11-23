/*------------------------------------------------------------------------*/
/* file: sci_xs2ps.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2ps routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2ps.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2ps(char *fname,unsigned long fname_len)
{
  return xs2file( fname, "Pos", fname_len, 3 ) ;
}
/*--------------------------------------------------------------------------*/
