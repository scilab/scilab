/*------------------------------------------------------------------------*/
/* file: sci_xg2ps.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xg2ps routine                                     */
/*------------------------------------------------------------------------*/


#include "sci_xg2ps.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xg2ps(char *fname,unsigned long fname_len)
{
  return xs2file( fname, "Pos", fname_len, 3 ) ;
}
/*--------------------------------------------------------------------------*/
