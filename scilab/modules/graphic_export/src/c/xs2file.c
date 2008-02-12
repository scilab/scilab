/*------------------------------------------------------------------------*/
/* file: xs2file.c                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray,                                               */
/*           Jean-Baptiste Silvy                                          */
/* desc : interface for xs2file routine                                   */
/*------------------------------------------------------------------------*/

#include "xs2file.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "WindowList.h"
#include "IsAScalar.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "SetJavaProperty.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
  CheckRhs(2,3);

  CheckLhs(0,1);
  CheckRhs(2,2);
  if ( (GetType(2) == sci_strings) && IsAScalar(1) )
  {
    integer m1,n1,l1;
    int figurenum=-1;
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
    figurenum = *istk(l1);
    if (figurenum>=0)
    {
      char * fileName=NULL;
      GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
      fileName = cstk(l1);

      /* Call the function for exporting file */
      exportToFile(getFigureFromIndex(figurenum), fileName, fileType);
    }
    else
    {
      Scierror(999,_("%s: Wrong input argument: %s expected.\n"),fname,">=0");
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
      Scierror(999,_("%s: Wrong type for second input argument: String expected.\n"),fname);
      return 0;
    }
  }
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
