/*------------------------------------------------------------------------*/
/* file: sci_xs2fig.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2fig routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2fig.h"
#include "xs2file.h"
#include "sciprint.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_xs2fig(char *fname,unsigned long fname_len)
{
  //return xs2file( fname, NO_EXPORT ) ;
  sciprint(_("%s: This feature has not been implemented.\n"),fname);
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
