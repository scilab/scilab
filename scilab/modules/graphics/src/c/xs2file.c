/*------------------------------------------------------------------------*/
/* file: xs2file.c                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Philippe Chancelier,                                    */
/*           Fabrice Leray,                                               */
/*           Jean-Baptiste Silvy                                          */
/* desc : interface for xs2file routine                                   */
/*------------------------------------------------------------------------*/

#include "xs2file.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "graphicSession.h"
#include "DrawObjects.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "getcolordef.h"
#include "localization.h"
#include "SetJavaProperty.h"

/*--------------------------------------------------------------------------*/
int xs2file(char * fname, ExportFileType fileType )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,flagx = -1,iwin;
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

  return 0;
}
/*--------------------------------------------------------------------------*/
int scig_toPs( integer win_num, integer colored, char * bufname, char * driver )
{
  integer ierr;
  int save_xinitxend_flag = isGraphicSessionOpened();
  sciPointObj * curFig = sciGetCurrentFigure ();
  integer bg ;
  double * curColorMap = NULL ;
  double * bwColorMap  = NULL ; /* allocated if black and white needed */
  int colorMapSize = sciGetNumColors( curFig ) ;

  ierr = 0;


  if( curFig == (sciPointObj *) NULL )
  {
    Scierror(999,_("No current graphic window %d found for exporting to %s.\n"),win_num,driver);
    return ierr;
  }

  /* Rajout F.Leray 06.04.04 */
  bg = sciGetBackground(curFig);

  if (ierr != 0)
  {
    return ierr ;
  }

  if ( colored == 0 )
  {
    /* change the colormap to a bw one */
    curColorMap = MALLOC( 3 * colorMapSize * sizeof(double) ) ;
    bwColorMap  = MALLOC( 3 * colorMapSize * sizeof(double) ) ;
    sciGetColormap( curFig, curColorMap ) ;
    convertColorMap2BW( bwColorMap, curColorMap, colorMapSize ) ;
    sciSetColormap( curFig, bwColorMap, colorMapSize, 3 ) ;
    FREE( bwColorMap ) ;
    bwColorMap = NULL ;
  }

  closeGraphicSession() ; /* we force to draw */
  sciDrawObj(curFig);


  if ( save_xinitxend_flag )
  {
    openGraphicSession() ;
  }
  else
  {
    closeGraphicSession() ;
  }

  if ( colored == 0 )
  {
    sciSetColormap( curFig, curColorMap, colorMapSize, 3 ) ;
    FREE( curColorMap ) ;
    curColorMap = NULL ;
  }

  return ierr;
}
/*--------------------------------------------------------------------------*/
int C2F(xg2psofig)(char *fname, integer *len, integer *iwin, integer *color, char *driver, long int l1, long int l2)
{
  int sc;
  if ( *color == -1 ) 
    getcolordef(&sc);
  else 
    sc= *color;
  /*scig_toPs(*iwin,sc,fname,driver);*/
  return scig_toPs(*iwin,sc,fname,driver); /* why twice ???? */
}
/*--------------------------------------------------------------------------*/
void exportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType)
{
  sciJavaExportToFile(pFigure, fileName, fileType);
}
/*--------------------------------------------------------------------------*/
