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
/*--------------------------------------------------------------------------*/
int xs2file( char * fname, char * dr, unsigned long fname_len, unsigned long dr_len )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,flagx = -1,iwin;
  CheckRhs(2,3);

  /* the window number */ 
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1) ;
  iwin = (integer) *stk(l1) ;
  /* the file name */ 
  GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
  /* color or n & b */ 
  if ( Rhs >= 3 )
  {
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
    CheckScalar(3,m3,n3) ;
    flagx = (integer) *stk(l3) ;
  }
  /* nouveau graphique ?????*/
  C2F(xg2psofig)(cstk(l2),&m2,&iwin,&flagx,dr,bsiz,dr_len);
  LhsVar(1)=0;
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
