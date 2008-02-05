/*------------------------------------------------------------------------*/
/* file: sci_xs2emf.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet                                                 */
/* desc : interface for xs2emf routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2emf.h"
#include "xs2file.h"
#include "sciprint.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_xs2emf( char * fname, unsigned long fname_len )
{
  //return xs2file(fname, NO_EXPORT );
  sciprint(_("%s: This feature has not been implemented.\n"),fname);
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
