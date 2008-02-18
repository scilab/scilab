/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: StringBox.c                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#include "StringBox.h"
#include "GetProperty.h"
#include "Axes.h"
#include "math_graphics.h"
#include "PloEch.h"
#include "MALLOC.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"

/*-------------------------------------------------------------------------------*/
void getStringBbox( char * string, int center[2], int rect[4] )
{
  center[0] = 0;
  center[1] = 0;
  rect[0] = 0;
  rect[1] = 0;
  rect[2] = 0;
  rect[3] = 0;
}
/*-------------------------------------------------------------------------------*/
void callXstringL( char * string, int posX, int posY, int boundingRect[4] )
{
  //sciPointObj * parentSubWin = sciGetFirstTypedSelectedSon( sciGetCurrentFigure(), SCI_SUBWIN ) ;

  /* now we can call xstringl */
  boundingRect[0] = 0;
  boundingRect[1] = 0;
  boundingRect[2] = 0;
  boundingRect[3] = 0;

}
/*-------------------------------------------------------------------------------*/
void getStringBox( char   ** text         ,
                  double    textPos[2]   ,
                  int       textDim[2]   ,
                  BOOL      autoSize     ,
                  double    userSize[2]  ,
                  double    angle        ,
                  int       fontId       ,
                  int       fontSize     ,
                  double    corners[4][2] )
{
  int pos[2] ;
  int textSize[2] = {0,0} ;
  int corn[4][2] ;
  StringMatrix * strings = newCompleteMatrix( (void **) text, textDim[0], textDim[1] ) ;

  /* take coordinates in pixels */
  pos[0] = XDouble2Pixel( textPos[0] ) ;
  pos[1] = YDouble2Pixel( textPos[1] ) ;

  if ( !autoSize )
  {
    textSize[0] = WDouble2Pixel( textPos[0], userSize[0] ) ;
    textSize[1] = HDouble2Pixel( textPos[1], userSize[1] ) ;
  }

  /* NULL because we don't need the position of each string */
  getStringsPositions( strings, &fontId, &fontSize, pos, autoSize, textSize, FALSE, angle, NULL, corn ) ;

  /* take everything back to user coordinates */
  /* to retrieve exactly the first corner as in stringl we take the input */
  corners[0][0] = XDPixel2Double( corn[0][0] ) ;
  corners[0][1] = YDPixel2Double( corn[0][1] ) ;

  corners[1][0] = XDPixel2Double( corn[1][0] ) ;
  corners[1][1] = YDPixel2Double( corn[1][1] ) ;

  corners[2][0] = XDPixel2Double( corn[2][0] ) ;
  corners[2][1] = YDPixel2Double( corn[2][1] ) ;

  corners[3][0] = XDPixel2Double( corn[3][0] ) ;
  corners[3][1] = YDPixel2Double( corn[3][1] ) ;

  /* we don't need the matrix anymore, but the text is needed */
  desallocateMatrix( strings ) ;

}
/*-------------------------------------------------------------------------------*/
void getStringsRect( StringMatrix  * strMat            ,
                    int             textPos[2]        ,
                    Vect2iMatrix  * stringPosition    ,
                    int             boundingBox[4][2]  )
{
  /* the space size between two consecutive strings in a row */
  int hSpace = getStringHorizontalSpace() ;
  int vSpace = getStringVerticalSpace()   ;
  int * rowHeight ; /* vectors containing height and width of column of the array*/
  int * colWidth  ;

  int i ;
  int j ;
  int nbRow = getMatNbRow( strMat ) ;
  int nbCol = getMatNbCol( strMat ) ;
  int rect[4] ;
  int curHeight = 0 ; /* height in pixels of the current row */
  int curWidth  = 0 ; /* lenght of the longest row in pixels */


  /* initialize arrays */
  rowHeight = MALLOC( (nbRow + 1) * sizeof( int ) ) ;
  colWidth  = MALLOC( (nbCol + 1) * sizeof( int ) ) ;

  /* the tricky part */

  /* we first compute the width of each row  of the array */
  /* The width is given by the tallest string of each row */

  rowHeight[nbRow] = textPos[1] ;

  /* begin with the lower left string which is at position textPos */
  for ( i = nbRow - 1 ; i >= 0 ; i-- )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      getStringBbox( getStrMatElement( strMat, i, j ), textPos, rect ) ;
      curHeight = Max( curHeight, rect[3] ) ;
    }
    /* the height of the current column is the max of the height its tallest string + vSpace */
    rowHeight[i] = rowHeight[i+1] - curHeight - vSpace ;
    curHeight = 0 ;
  }

  /* same for columns */
  colWidth[0] = textPos[0] ;

  for ( j = 0 ; j < nbCol ; j++ )
  {
    for ( i = nbRow - 1 ; i >= 0 ; i-- )
    {
      getStringBbox( getStrMatElement( strMat, i, j ), textPos, rect ) ;
      curWidth = Max( curWidth, rect[2] ) ;
    }
    colWidth[j+1] = colWidth[j] + curWidth + hSpace ;
    curWidth = 0 ;
  }

  /* now fill the matrix */
  if ( stringPosition != NULL )
  {
    for ( i = 0 ; i <= nbRow ; i++ )
    {
      for ( j = 0 ; j <= nbCol ; j++ )
      {
        int intersect[2] = { colWidth[j], rowHeight[i] } ;
        copyVect2iMatElement( stringPosition, i, j, intersect ) ;
      }
    }
  }

  /* get the bounding box */
  boundingBox[0][0] = textPos[0] ;
  boundingBox[0][1] = textPos[1] ;

  boundingBox[1][0] = boundingBox[0][0] ;
  boundingBox[1][1] = rowHeight[0] ;

  boundingBox[2][0] = colWidth[nbCol] ;
  boundingBox[2][1] = boundingBox[1][1] ;

  boundingBox[3][0] = boundingBox[2][0] ;
  boundingBox[3][1] = boundingBox[0][1] ;

  FREE( rowHeight ) ;
  FREE( colWidth  ) ;

}
/*-------------------------------------------------------------------------------*/
void getStringsRectSized( StringMatrix  * strMat           ,
                          int             textPos[2]       ,
                          Vect2iMatrix  * stringPosition   ,
                          int             boundingBox[4][2],
                          int             userSize[2]      ,
                          int           * newFontSize       )
{
  //integer curFont[2] = {0,0}  ;
  //integer fontSizeOne = 1 ;
  double homothFactors[2] ;

  /* first get the automatic array with font 1 */

  /* get the current font */
  /* we need to change the defaut font before using xstringl */
  /* C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L); */

  /* set the new font */
  /* C2F(dr)("xset","font",&(curFont[0]),&fontSizeOne,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */

  /* get the displaying array */
  getStringsRect( strMat, textPos, stringPosition, boundingBox ) ;

  /* get the homothety factors */
  /* horizontal : userWidth / bbWidth */
  homothFactors[0] = ((double) userSize[0]) / ( boundingBox[2][0] - boundingBox[0][0] ) ;
  homothFactors[1] = ((double) userSize[1]) / ( boundingBox[0][1] - boundingBox[2][1] ) ;

  /* apply homothety on the matrix and boundingbox */
  iHomothety2D( boundingBox[1], boundingBox[0], homothFactors, boundingBox[1] ) ;
  iHomothety2D( boundingBox[2], boundingBox[0], homothFactors, boundingBox[2] ) ;
  iHomothety2D( boundingBox[3], boundingBox[0], homothFactors, boundingBox[3] ) ;

  /* we must tkae the min from both direction to avoid strings to go over the lines. */
  if ( stringPosition != NULL )
  {
    homothVect2iMatrix( stringPosition, boundingBox[0], homothFactors ) ;
    *newFontSize = computeSuitableFont( strMat, stringPosition ) ;
  }

  /* return to the previous font */
  /* C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */
}
/*-------------------------------------------------------------------------------*/
void getStringsPositions( StringMatrix  * strMat        ,
                          int           * fontId        ,
                          int           * fontSize      ,
                          int             textPos[2]    ,
                          BOOL            autoSize      ,
                          int             textSize[2]   ,
                          BOOL            centerPos     ,
                          double          angle         ,
                          Vect2iMatrix  * stringPosition,
                          int             boundingBox[4][2] )
{
  //integer curFont[2] = {0, 0}  ;
  int trans[2] ;

  if ( autoSize )
  {
    /* get the current font */
    /* we need to change the defaut font before using xstringl */
    /* C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L); */

    /* set the new font */
    /* C2F(dr)("xset","font",fontId,fontSize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */

    /* compute the stringPosition and bounding box in pixels without rotation */
    getStringsRect( strMat, textPos, stringPosition, boundingBox ) ;

    /* return to the previous font */
    /* C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */
  }
  else
  {
    getStringsRectSized( strMat, textPos, stringPosition, boundingBox, textSize, fontSize ) ;
  }

  /* for now we have computes the matrix as if its position was corresponding to its */
  /* lower-left vertice. */
  /* we must now translate the points depending on where is really the center relatively */
  /* to the array. */
  getStringPositionTranslation( centerPos, textSize, boundingBox, trans ) ;
  if ( stringPosition != NULL )
  {
    translateVect2iMatrix( stringPosition, trans ) ;
  }
  translateBoundingBox( boundingBox, trans ) ;

  /* then turn everything, we need to turn them in pixels because of logarithmic scale */
  if ( Abs( angle ) > EPSILON )
  {
    if ( stringPosition != NULL )
    {
      rotateVect2iMatrix( stringPosition, textPos, angle ) ;
    }
    rotateBoundingBox( boundingBox, textPos, angle ) ;
  }

}
/*-------------------------------------------------------------------------------*/
void getTextBoundingBox( sciPointObj * pText        ,
                        int           cornPix[4][2],
                        double        corners[4][2] )
{
  int           fontId   = sciGetFontStyle( pText ) ;
  int           fontSize = (int) sciGetFontSize( pText ) ;
  double        angle    = sciGetFontOrientation(pText); 
  int           position[2] ;
  double        textPos[3]  ;
  double        userSize[2] ;
  int           textSize[2] ;
  sciText     * ppText   = pTEXT_FEATURE( pText ) ;
  sciPointObj * parentSW = sciGetParentSubwin( pText ) ;

  /* transform the position in pixels */
  /* we don't take the axes reverse into account. This has obviously no meaning for text.*/
  textPos[0] = ppText->x ;
  textPos[1] = ppText->y ;
  textPos[2] = ppText->z ;

  if ( sciGetIs3d( pText ) )
  {
    /* normal case */
    getPixelCoordinates( parentSW, textPos, position ) ;
  }
  else
  {
    /* special for label, use of the 2d scale */
    position[0] = XDouble2Pixel( textPos[0] ) ;
    position[1] = YDouble2Pixel( textPos[1] ) ;
  }

  sciGetUserSize( pText, &(userSize[0]), &(userSize[1]) ) ;

  /* We take the size in 2d. */
  textSize[0] = PixelWidth2d(  parentSW, ppText->x, userSize[0] ) ;
  textSize[1] = PixelHeight2d( parentSW, ppText->y, userSize[1] ) ;

  if ( cornPix == NULL )
  {
    int corn[4][2] ;
    /* NULL because we don't need the position of each string */
    getStringsPositions( sciGetText( pText )     ,
      &fontId                 ,
      &fontSize               ,
      position                ,
      sciGetAutoSize( pText ) ,
      textSize                ,
      sciGetCenterPos( pText ),
      angle                   ,
      NULL                    ,
      corn                     ) ;
    if ( corners != NULL )
    {
      /* take everything back to user coordinates */
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = XDPixel2Double( corn[0][0] ) ;
      corners[0][1] = YDPixel2Double( corn[0][1] ) ;

      corners[1][0] = XDPixel2Double( corn[1][0] ) ;
      corners[1][1] = YDPixel2Double( corn[1][1] ) ;

      corners[2][0] = XDPixel2Double( corn[2][0] ) ;
      corners[2][1] = YDPixel2Double( corn[2][1] ) ;

      corners[3][0] = XDPixel2Double( corn[3][0] ) ;
      corners[3][1] = YDPixel2Double( corn[3][1] ) ;
    }
  }
  else
  {
    /* NULL because we don't need the position of each string */
    getStringsPositions( sciGetText( pText )     ,
      &fontId                 ,
      &fontSize               ,
      position                ,
      sciGetAutoSize( pText ) ,
      textSize                ,
      sciGetCenterPos( pText ),
      angle                   ,
      NULL                    ,
      cornPix                  ) ;

    if ( corners != NULL )
    {
      /* take everything back to user coordinates */
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = XDPixel2Double( cornPix[0][0] ) ;
      corners[0][1] = YDPixel2Double( cornPix[0][1] ) ;

      corners[1][0] = XDPixel2Double( cornPix[1][0] ) ;
      corners[1][1] = YDPixel2Double( cornPix[1][1] ) ;

      corners[2][0] = XDPixel2Double( cornPix[2][0] ) ;
      corners[2][1] = YDPixel2Double( cornPix[2][1] ) ;

      corners[3][0] = XDPixel2Double( cornPix[3][0] ) ;
      corners[3][1] = YDPixel2Double( cornPix[3][1] ) ;
    }
  }

}
/*-------------------------------------------------------------------------------*/
void getTextAabb( sciPointObj * pText        ,
                 int           rectPix[4]   ,
                 int           cornPix[4][2] )
{
  if ( cornPix == NULL )
  {

    getTextBoundingBox( pText, cornPix, NULL ) ;

    if ( rectPix != NULL )
    {
      rectPix[0] = Min( cornPix[0][0], Min( cornPix[1][0], Min( cornPix[2][0], cornPix[3][0] ) ) ) ;
      /* in pixel the bottom left point has the maximal y value */
      rectPix[1] = Max( cornPix[0][1], Max( cornPix[1][1], Max( cornPix[2][1], cornPix[3][1] ) ) ) ;
      rectPix[2] = Max( abs( cornPix[2][0] - cornPix[0][0] ), abs( cornPix[3][0] - cornPix[1][0] ) ) ;
      rectPix[3] = Max( abs( cornPix[2][1] - cornPix[0][1] ), abs( cornPix[3][1] - cornPix[1][1] ) ) ;
    }
  }
  else
  {
    getTextBoundingBox( pText, cornPix, NULL ) ;

    if ( rectPix != NULL )
    {
      rectPix[0] = Min( cornPix[0][0], Min( cornPix[1][0], Min( cornPix[2][0], cornPix[3][0] ) ) ) ;
      /* in pixel the bottom left point has the maximal y value */
      rectPix[1] = Max( cornPix[0][1], Max( cornPix[1][1], Max( cornPix[2][1], cornPix[3][1] ) ) ) ;
      rectPix[2] = Max( abs( cornPix[2][0] - cornPix[0][0] ), abs( cornPix[3][0] - cornPix[1][0] ) ) ;
      rectPix[3] = Max( abs( cornPix[2][1] - cornPix[0][1] ), abs( cornPix[3][1] - cornPix[1][1] ) ) ;
    }
  }

}
/*-------------------------------------------------------------------------------*/

void rotateBoundingBox( int boundingBox[4][2], int center[2], double angle )
{
  if ( Abs( angle ) > EPSILON )
  {
    double cosAngle = cos( angle ) ;
    double sinAngle = sin( angle ) ;
    iRotate2Dim( boundingBox[0], center, cosAngle, sinAngle, boundingBox[0] ) ;
    iRotate2Dim( boundingBox[1], center, cosAngle, sinAngle, boundingBox[1] ) ;
    iRotate2Dim( boundingBox[2], center, cosAngle, sinAngle, boundingBox[2] ) ;
    iRotate2Dim( boundingBox[3], center, cosAngle, sinAngle, boundingBox[3] ) ;
  }
}
/*-------------------------------------------------------------------------------*/
void translateBoundingBox( int boundingBox[4][2], int trans[2] )
{
  iTranslate2D( boundingBox[0], trans, boundingBox[0] ) ;
  iTranslate2D( boundingBox[1], trans, boundingBox[1] ) ;
  iTranslate2D( boundingBox[2], trans, boundingBox[2] ) ;
  iTranslate2D( boundingBox[3], trans, boundingBox[3] ) ;

}
/*-------------------------------------------------------------------------------*/
int getStringVerticalSpace( void )
{
  int rect[4] ;
  int pos[2] = {0,0} ;
  getStringBbox( " ", pos, rect ) ;
  return rect[3] ;
}
/*-------------------------------------------------------------------------------*/
int getStringHorizontalSpace( void )
{
  int rect[4] ;
  int pos[2] = {0,0} ;
  /* return the width of a space */
  getStringBbox( " ", pos, rect ) ;
  return rect[2] ;
}
/*-------------------------------------------------------------------------------*/
BOOL isFittingInCell( char * string, int cellWidth, int cellHeight )
{
  int rect[4] ;
  int pos[2] = {0,0} ;

  /* get the size of the string */
  getStringBbox( string, pos, rect ) ;

  if ( rect[2] < cellWidth && rect[3] < cellHeight )
  {
    return TRUE ;
  }
  return FALSE ;
}
/*-------------------------------------------------------------------------------*/
void getStringPositionTranslation( BOOL centeredPos, int textSize[2], int bbox[4][2], int trans[2] )
{
  if ( centeredPos )
  {
    trans[0] = ( textSize[0] + bbox[0][0] - bbox[2][0] ) / 2 ;
    trans[1] = ( -textSize[1] + bbox[0][1] - bbox[2][1] ) / 2 ;
  }
  else
  {
    trans[0] = 0 ;
    trans[1] = 0 ;
  }
}
/*-------------------------------------------------------------------------------*/
int computeSuitableFont( StringMatrix  * strMat, Vect2iMatrix  * stringPosition )
{
  int nbRow = getMatNbRow( strMat ) ;
  int nbCol = getMatNbCol( strMat ) ;
  int largestFont = 0 ;
  int smallestFont = 1 ;
  int i ;
  int j ; 
  //integer curFont[2] = {0,0} ;

  /* C2F(dr)("xget","font",&verbose,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L) ; */

  /* the largest font is the minimum of all largest font which can be used for each cells */
  /* initializeit with the largest font the driver can display */
  /* C2F(dr)( "xfontmxs","", &smallestFont, &largestFont, PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L ) ; */

  /* set the largest font */
  /* C2F(dr)("xset","font",&(curFont[0]),&largestFont,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */

  for ( i = 0 ; i < nbRow ; i++ )
  {
    for ( j = 0 ; j < nbCol ; j++ )
    {
      int  * blCorner = getVect2iMatElement( stringPosition, i    , j     ) ;
      int  * urCorner = getVect2iMatElement( stringPosition, i + 1, j + 1 ) ;
      char * string   = getStrMatElement( strMat, i, j ) ;
      int width  = urCorner[0] - blCorner[0] ;
      int height = urCorner[1] - blCorner[1] ;
      while ( !isFittingInCell( string, width, height ) )
      {
        largestFont-- ;
        if ( largestFont == smallestFont )
        {
          /* we reach the minimum size, no need to continue */
          /* C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L) ; */
          return smallestFont ;
        }
        /* set the new font */
        /* C2F(dr)("xset","font",&(curFont[0]),&largestFont,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L); */
      }
    }
  }

  /* return to the previous font */
  /* C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  */
  return largestFont ;

}
/*-------------------------------------------------------------------------------*/
