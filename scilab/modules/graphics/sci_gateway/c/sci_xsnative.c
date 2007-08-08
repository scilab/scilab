/*------------------------------------------------------------------------*/
/* file: sci_xsnative.c                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : interface for clc routine                                       */
/*------------------------------------------------------------------------*/

#include "sci_xsnative.h"

#ifdef _MSC_VER
#include <Windows.h>
#undef Top
#endif

#include "stack-c.h"

#ifdef _MSC_VER

#include "Os_specific.h"
#include "../../gui/src/c/wsci/Errors.h"
#include "../../gui/src/c/wsci/ExportBmpEmf.h"
#include "GetProperty.h"
#include "WindowList.h"
#include "MALLOC.h"
#include "cluni0.h"

/*-----------------------------------------------------------------------------------*/
static int NumBMP=0;
static char DefaultFilenameTests[PATH_MAX]="IMG";
#define ExtensionTests ".bmp"
/*-----------------------------------------------------------------------------------*/
int sci_xsnative( char * fname, unsigned long fname_len )
{
  static int l1, m1, n1;
  char FilenameBMP[PATH_MAX];
  integer iflag =0,ids,num,un=1;
  int *ArrayWGraph=NULL;
  int i=0;

  switch( Rhs )
  {

  case 0:
    {
      /*DefaultFilenameTests*/
    }
    break;

  case 1:
    {
      if ( GetType(1) == 1 ) 
      {
        GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
        NumBMP=*istk(l1);
      }
      else if( GetType(1) == 10 )
      {
        char *Input;
        int out_n;
        GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
        Input=cstk(l1);

        C2F(cluni0)(Input,DefaultFilenameTests, &out_n,(long)strlen(Input),PATH_MAX);
        NumBMP=0;
      }
      else
      {
        Scierror(999,MSG_ERROR35);
        LhsVar(1)=0;
        return 0;
      }

    }
    break;

  default:
    {
      Scierror(999,MSG_ERROR35);
      LhsVar(1)=0;
      return 0;
    }
    break;
  }


  sciGetIdFigure (&ids,&num,&iflag);
  iflag = 1;
  ArrayWGraph = MALLOC( sizeof(int) * num );
  sciGetIdFigure (ArrayWGraph,&num,&iflag);	

  for (i=0;i<num;i++)
  {
    struct BCG *ScilabGC=NULL;

    wsprintf(FilenameBMP,"%s%d.bmp",DefaultFilenameTests,NumBMP);

    ScilabGC = getWindowXgcNumber (ArrayWGraph[i]);

    if (ScilabGC != (struct BCG *) 0)
    {
      ExportBMP(ScilabGC,FilenameBMP);
      NumBMP++;
    }
  }

  FREE(ArrayWGraph);
  ArrayWGraph=NULL;


  LhsVar(1)=0;
  return 0;

}
/*-----------------------------------------------------------------------------------*/
#else
int sci_xsnative( char * fname, unsigned long fname_len )
{
  Scierror(999,"Not yet implemented\n") ;
  LhsVar(1)=0;
  C2F(putlhsvar)() ;
  return 0;
}
#endif
