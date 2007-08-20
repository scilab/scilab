/*------------------------------------------------------------------------*/
/* file: sci_xs2bmp.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors :  Allan Cornet                                                */
/* desc : interface for xs2bmp routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xs2bmp.h"
#include "sciprint.h"
#include "machine.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
extern int IsAScalar(int RhsNumber);
/*-----------------------------------------------------------------------------------*/
int sci_xs2bmp( char * fname, unsigned long fname_len )
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
      // bOK=Interface_XS2BMP(figurenum,FileName);
    }
    else
    {
      Scierror(999,"%s: First Argument. Must be >=0.\r\n",fname);
      return 0;
    }

  }
  else
  {
    if ( IsAScalar(1) )
    {
      Scierror(999,"%s: First Argument. Must be a integer scalar.\r\n",fname);
      return 0;
    }
    if ( GetType(2) != sci_strings)
    {
      Scierror(999,"%s: Second Argument. Must be a string.\r\n",fname);
      return 0;
    }
  }

#else
  sciprint("%s: Only for Windows.\r\n",fname);
  bOK=0;
#endif
  return bOK;
}
/*-----------------------------------------------------------------------------------*/
