/*------------------------------------------------------------------------*/
/* file: sci_xs2emf.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet                                                 */
/* desc : interface for xs2emf routine                                    */
/*------------------------------------------------------------------------*/

#ifdef _MSC_VER
#include "sci_xs2emf.h"
#endif
#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "IsAScalar.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xs2emf( char * fname, unsigned long fname_len )
{
  int bOK=0;
#ifdef _MSC_VER

  CheckLhs(0,1);
  CheckRhs(2,2);
  if ( (GetType(2) == sci_strings) && IsAScalar(1) )
  {
    integer m1,n1,l1;
    int figurenum=-1;
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
    figurenum=*istk(l1);
    if (figurenum>=0)
    {
      char *FileName=NULL;
      GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
      FileName=cstk(l1);
    }
    else
    {
      Scierror(999,_("%s: Wrong input argument: >=0 expected.\n"),fname);
      return 0;
    }

  }
  else
  {
    if ( IsAScalar(1) )
    {
      Scierror(999,_("%s: Wrong type for first input argument: integer scalar expected.\n"),fname);
      return 0;
    }
    if ( GetType(2) != sci_strings)
    {
      Scierror(999,_("%s: Wrong type for second input argument. String expected.\n"),fname);
      return 0;
    }
  }

#else
  sciprint(_("%s: Only for Windows.\n"),fname);
  bOK=0;
#endif
  return bOK;
}
/*--------------------------------------------------------------------------*/
