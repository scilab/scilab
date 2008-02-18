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
/* file: StringBox.h                                                      */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#ifndef _STRING_BOX_H_
#define _STRING_BOX_H_

#include "machine.h"
#include "StringMatrix.h"
#include "Vect2Matrix.h"
#include "ObjectStructure.h"

/*----------------------------------------------------------------------------------*/
/**
 * return the bounding rectangle of a string.
 * @param string bounded string
 * @param center Position where the string ought to be displayed.
 * @param rect   vector containing the upper left point and the width and height of the
 *               bounding box : [x,y,w,h].
 */
void getStringBbox( char * string, int center[2], int rect[4] ) ;

/**
 * This is a wrapping of the function C2F(dr)("xstringl",... It should be prefered to xstringl
 * because it is working even in drawlater mode. The parent Subwindow of the text must be the
 * current axes.
 */
void callXstringL( char * string, int posX, int posY, int boundingRect[4] ) ;
/*----------------------------------------------------------------------------------*/
/**
 * compute the four corners of a text
 * works only in 2D
 */
void getStringBox( char   ** text         ,
                   double    textPos[2]   ,
                   int       textDim[2]   ,
                   BOOL      autoSize     ,
                   double    userSize[2]  ,
                   double    angle        ,
                   int       fontId       ,
                   int       fontSize     ,
                   double    corners[4][2] ) ;

/**
 * Compute an array of cells in which a set of strings can be displayed. The size of the
 * array is automatically computed using the current font.
 * This routine does not compute any rotation.
 * @param strMat Set of strings.
 * @param textPos Position of the upper left point of the string array.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells.
 */
void getStringsRect( StringMatrix  * strMat            ,
                     int             textPos[2]        ,
                     Vect2iMatrix  * stringPosition    ,
                     int             boundingBox[4][2]  ) ;

/**
 * Compute an array of cells in which a set of strings can be displayed when the size
 * (width and height) of the array are specified by user. This routine does not compute
 * any rotation.
 * @param strMat Set of strings.
 * @param textPos Position of the upper left point of the string array.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells.
 * @param userSize width and height of the array of cells.
 * @param newFontSize font size which would fit the array best using the strings in strMat.
 */
void getStringsRectSized( StringMatrix  * strMat           ,
                          int             textPos[2]       ,
                          Vect2iMatrix  * stringPosition   ,
                          int             boundingBox[4][2],
                          int             userSize[2]      ,
                          int           * newFontSize       ) ;

/**
 * Compute an array of cells in which each string will fit. The bounding box of the
 * finally displayed string is also computed. The generated array of cells can the be used
 * for the display with the drawStringsInPosition routine.
 * @param strMat    Set of string.
 * @param fontId    specify the fontId of the strings.
 * @param fontSize  if autoSize is on then specify the size of the strings. Otherwise,
 *                  this returns the size which should be used to display the strings in
 *                  the cell array.
 * @param textPos   Position of the upper left point of the string array and center
 *                  of rotation.
 * @param autoSize  If TRUE, the size of the text is displayed usign the specified font.
 *                  If flase the width and height of the text are specified by the user.
 * @param textSize  if autoSize is false, specify the size of the rectangle in which the
 *                  text must fit.
 * @param angle     Angle for the rotation of the strings.
 * @param centerPos Specify where is the rotation center of the string. If TRUE in the middle
 *                  of the array, otherwise in the bottom-left.
 * @param stringPosition matrix containing the corners of the cells. Element (i,j) of this
 *                       matrix correspond to the upper-left point of the string (i,j).
 *                       It size must be equal to the size of strMat plus one in each
 *                       dimension. If NULL, not computed.
 * @param boundingBox position of the four corners of the bounding box surrounding the
 *                    array of cells. The points are given clockwise.
 */
void getStringsPositions( StringMatrix  * strMat        ,
                          int           * fontId        ,
                          int           * fontSize      ,
                          int             textPos[2]    ,
                          BOOL            autoSize      ,
                          int             textSize[2]   ,
                          BOOL            centerPos     ,
                          double          angle         ,
                          Vect2iMatrix  * stringPosition,
                          int             boundingBox[4][2] ) ;
/*----------------------------------------------------------------------------------*/
/**
 * Compute the bounding box of a text object.
 * @param pText   Pointer on the object to surround.
 * @param cornPix Bounding box in pixel coordinates. If NULL, not computed.
 * @param corners Bounding box in user coordinates. If NULL, not computed.
 */
void getTextBoundingBox( sciPointObj * pText        ,
                         int           cornPix[4][2],
                         double        corners[4][2] ) ;

/**
 * Compute the axis aligned bounding box of a text object.
 * @param pText   Pointer on the object to surround.
 * @param rectPix Bounding box (x,y,w,h) in pixel coordinates. If NULL, not computed.
 *                (x,y) is the coordinates of the bottom left vertice.
 * @param conrPix The four corners of the real bounding box. If NULL, not computed.
 */
void getTextAabb( sciPointObj * pText        ,
                  int           rectPix[4]   ,
                  int           cornPix[4][2] ) ;
/*----------------------------------------------------------------------------------*/
/**
 * Rotate four points around the first one.
 * @param boundingBox the four 2D points.
 * @param angle rotation angle in radian.
 */
void rotateBoundingBox( int boundingBox[4][2], int center[2], double angle ) ;

/**
 * Translae four points around the first one.
 * @param boundingBox the four 2D points.
 * @param angle rotation angle in radian.
 */
void translateBoundingBox( int boundingBox[4][2], int trans[2] ) ;
/*----------------------------------------------------------------------------------*/
 /**
 * returns the minimal space between too consecutive strings horizontally.
 */
int getStringHorizontalSpace( void ) ;

/**
 * returns the minimal space between too consecutive strings vertically.
 */
int getStringVerticalSpace( void ) ;

/*----------------------------------------------------------------------------------*/
/**
 * Check if a string can be displayed in a cell with a certain width and height.
 */
BOOL isFittingInCell( char * string, int cellWidth, int cellHeight ) ;

/**
 * Get the translation which should be applied to the text depending on the meaning of
 * its position.
 * @param centeredPos specify the meaning of the position of the text. If FALSE, the
 *                    position corresponds to the position of the lower left point. So
 *                    nothing needs to be changed. If TRUE, the center is the position
 *                    corresponds to the center of the text bounding box and then a
 *                    translation must be performed.
 * @param textSize width and height wanted for the text.
 * @param bbox The four corners of the computed boundign box of the text. The first corner
 *             should correspond to the position of the text.
 * @param trans Translation to apply to the text.
 */
void getStringPositionTranslation( BOOL centeredPos, int textSize[2], int bbox[4][2], int trans[2] ) ;

/**
* get the largest font which can be used to fit the strings in the array.
* @param strMat Set of strings which try to fit in stringPosition.
* @param position of the cells in which the strings need to fit.
*/
int computeSuitableFont( StringMatrix  * strMat, Vect2iMatrix  * stringPosition ) ;
/*----------------------------------------------------------------------------------*/
#endif /* _STRING_BOX_H_ */
