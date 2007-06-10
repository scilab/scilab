/*------------------------------------------------------------------------*/
/* file: drawTextEntity.c                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a text handle                 */
/*------------------------------------------------------------------------*/

#include "drawTextEntity.h"
#include "Vect2Matrix.h"
#include "GetProperty.h"
#include "periScreen.h" /* should be removed */
#include "DrawObjects.h" /* should be removed */
#include "Xcall1.h"
#include "Axes.h"
#include "StringBox.h"
#include "math_graphics.h"
#include "sciprint.h"
#include "clipping.h"

/*----------------------------------------------------------------------------------------*/
int drawTextEntity( sciPointObj * pObj )
{
  int    v  = 0  ;
  double dv = 0. ;
  double anglestr = 0. ;
  int textProperties[6] ;
  int verb=0;
  int font_[2], cur_font_[2] ;
  int position[2] ;
  int textDim[2] ;
  double userSize[2] ;
  int    textSize[2] ;
  double textPos[3] ;
  Vect2iMatrix * bboxes ;
  int globalBbox[4][2] ;
  sciText * ppText =  pTEXT_FEATURE( pObj ) ;
  sciPointObj * parentSW = sciGetParentSubwin(pObj) ;

  if (!sciGetVisibility(pObj))
  {
    return 0 ;
  }



#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc ();
#endif
  if ( ppText->isclip )
  {
    sciClip(pObj);
  }

  /* transform the position in pixels */
  /* we don't take the axes reverse into account. This has obviously no meaning for text.*/
  textPos[0] = ppText->x ;
  textPos[1] = ppText->y ;
  textPos[2] = ppText->z ;

  if ( sciGetIs3d( pObj ) )
  {
    /* normal case */
    getPixelCoordinates( parentSW, textPos, position ) ;
  }
  else
  {
    /* for labels, the text is displayed using 2d scale. */
    position[0] = XDouble2Pixel( textPos[0] ) ;
    position[1] = YDouble2Pixel( textPos[1] ) ;
  }


  sciGetUserSize( pObj, &(userSize[0]), &(userSize[1]) ) ;


  /* We take the size in 2d. */
  textSize[0] = PixelWidth2d(  parentSW, ppText->x, userSize[0] ) ;
  textSize[1] = PixelHeight2d( parentSW, ppText->y, userSize[1] ) ;

  /* *10 parce que l'angle est conserve en 1/10eme de degre*/
  /* si c'est pas nimp tout ca.... */
  anglestr = (sciGetFontOrientation (pObj)/10); 	

  /* set the font */
  C2F(dr)("xget","font",&verb,cur_font_,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

  font_[0] = sciGetFontStyle (pObj);
  font_[1] = sciGetFontDeciWidth (pObj)/100;

  C2F(dr)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* get the bounding box of the text matrix */
  /* the matrix with the bounding box of each strings in the text object */
  sciGetTextSize( pObj, &(textDim[0]), &(textDim[1]) ) ;
  bboxes = newMatrix( textDim[0] + 1, textDim[1] + 1 ) ;


  /* we get the array not turned because the display will turn everything by itself */
  /* However, the bounding box needs to be turned after */
  getStringsPositions( sciGetText( pObj )     ,
    &font_[0]              ,
    &font_[1]              ,
    position               ,
    sciGetAutoSize( pObj ) ,
    textSize               ,
    sciGetCenterPos( pObj ),
    0.0                    ,
    bboxes                 ,
    globalBbox              ) ;

  /* font might have changed */
  C2F(dr)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* wether or not we draw and/or fill the box */
  /* no need to compute anything if both line_mode */
  /* and fill mode are false */
  /* Of course, the background should be drawn first */
  if(    sciGetIsBoxed (pObj)
    && ( sciGetIsFilled( pObj ) || sciGetIsLine( pObj ) ) )
  {
    int xm[4], ym[4],n=4;
    int close=1;

    /* we need to rotate the bounding box */
    rotateBoundingBox( globalBbox, position,  DEG2RAD(anglestr) ) ;

    xm[0] = globalBbox[0][0] ;
    xm[1] = globalBbox[1][0] ;
    xm[2] = globalBbox[2][0] ;
    xm[3] = globalBbox[3][0] ;

    ym[0] = globalBbox[0][1] ;
    ym[1] = globalBbox[1][1] ;
    ym[2] = globalBbox[2][1] ;
    ym[3] = globalBbox[3][1] ;



    /* draw the background */
    if ( sciGetIsFilled( pObj ) )
    {
      textProperties[0] = sciGetBackground( pObj ) ;
      C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);

      C2F (dr) ("xarea", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0, 5L,(integer)strlen("xv"));
    }

    /* draw the line around the box */
    if ( sciGetIsLine( pObj ) )
    {
      textProperties[0] = sciGetForeground( pObj ) ;
      /* draw a rectangle around the text */
      C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
      C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);

      C2F (dr) ("xlines", "xv", &n, xm, ym, &close, PI0, PI0, PD0, PD0, PD0, PD0,6L,2L);
    }
  }

#ifdef _MSC_VER
  flag_DO = MaybeSetWinhdc ();
#endif
  /* load the object foreground and dashes color */
  textProperties[0] = sciGetFontForeground (pObj);/*la dash est de la meme couleur que le foreground*/
  textProperties[2] = sciGetFontDeciWidth (pObj)/100;
  textProperties[3] = 0 ;
  textProperties[4] = sciGetFontStyle(pObj);
  textProperties[5] = 0 ;

  C2F (dr) ("xset", "dashes", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v, &dv,&dv, &dv, &dv, 5L, 6L);
  C2F (dr) ("xset", "foreground", textProperties, textProperties, textProperties+3, textProperties+3, textProperties+3, &v,&dv, &dv, &dv, &dv, 5L, 10L);
  /* C2F(dr)("xset","font",x+4,x+2,&v, &v, &v, &v,&dv, &dv, &dv, &dv, 5L, 4L); */

#ifdef _MSC_VER
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif

  drawStringsInPosition( sciGetText( pObj ), bboxes,position, DEG2RAD(anglestr), sciGetAlignment( pObj ) ) ;
  /* C2F(dr)("xstring",getStrMatElement(sciGetText(pObj),0,0),&x1,&yy1,PI0,&flagx,PI0,PI0,&anglestr, PD0,PD0,PD0,0L,0L); */


  C2F(dr)("xset","font",&cur_font_[0],&cur_font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  deleteMatrix( bboxes ) ;

  if ( ppText->isclip )
  {
    sciUnClip(pObj);
  }

#ifdef _MSC_VER
  if ( flag_DO == 1) ReleaseWinHdc ();
#endif

  return 0 ;

}
/*----------------------------------------------------------------------------------------*/
void drawStringsInPosition( StringMatrix     * strings    ,
                           Vect2iMatrix     * cellsArray ,
                           int                center[2]  ,
                           double             angle      ,
                           sciTextAlignment   align       )
{
  int nbRow = getMatNbRow( strings ) ;
  int nbCol = getMatNbCol( strings ) ;
  int i ;
  int j ;

  for ( i = 0 ; i < nbRow ; i++ )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      /* get the two extreme corners of the cell in which the text will be drawn */
      int  * blCorner = getVect2iMatElement( cellsArray, i    , j     ) ;
      int  * urCorner = getVect2iMatElement( cellsArray, i + 1, j + 1 ) ;
      char * string   = getStrMatElement( strings, i, j ) ;
      drawStringInCell( string, blCorner, urCorner, center, angle, align ) ;
    }
  }
}
/*----------------------------------------------------------------------------------------*/
void drawStringInCell( char             * string      ,
                      int                blCorner[2] ,
                      int                urCorner[2] ,
                      int                rotCenter[2],
                      double             angle       ,
                      sciTextAlignment   align        )
{
  int rect[4] ;
  int leftMargin ;
  int bottomMargin ;
  int pos[2] ;
  int flag = 0 ;
  double angleStr = RAD2DEG( angle ) ;
  /* get the size of the string */
  getStringBbox( string, blCorner, rect ) ;

  getStringMargins( &(rect[2]), blCorner, urCorner, align, &leftMargin, &bottomMargin ) ;

  /* get the position unturned */
  pos[0] = blCorner[0] + leftMargin   ;
  pos[1] = blCorner[1] + bottomMargin + rect[3] ;

  /* then turn it around the center */
  if ( Abs( angle ) > EPSILON )
  {
    iRotate2D( pos, rotCenter, angle, pos ) ;
  }
  /* then display it */
  C2F(dr)("xstring",string,&(pos[0]),&(pos[1]),PI0,&flag,PI0,PI0,&angleStr, PD0,PD0,PD0,0L,0L);
}                       
/*----------------------------------------------------------------------------------------*/
void getStringMargins( int                stringSize[2],
                      int                blCorner[2]  ,
                      int                urCorner[2]  ,
                      sciTextAlignment   align        ,
                      int              * lMargin      ,
                      int              * bMargin       )
{
  int cellWidth  = urCorner[0] - blCorner[0] ;
  int cellHeight = urCorner[1] - blCorner[1] ;

  switch( align )
  {
  case ALIGN_LEFT:
    *lMargin = 0 ;
    break ;
  case ALIGN_RIGHT:
    *lMargin = cellWidth - stringSize[0] ;
    break ;
  case ALIGN_CENTER:
    /* same margin for each side */
    *lMargin = ( cellWidth - stringSize[0] ) / 2 ;
    break ;
  default:
    sciprint("error unhandled alignment\n");
    *lMargin = -1 ;
    *bMargin = -1 ;
    break ;
  }
  *bMargin = ( cellHeight - stringSize[1] ) / 2 ;

}
/*----------------------------------------------------------------------------------------*/
