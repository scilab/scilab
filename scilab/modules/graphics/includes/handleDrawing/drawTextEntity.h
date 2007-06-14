/*------------------------------------------------------------------------*/
/* file: drawTextEntity.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a text handle                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_TEXT_ENTITY_H_
#define _DRAW_TEXT_ENTITY_H_

#include "ObjectStructure.h"
#include "StringMatrix.h"
#include "Vect2Matrix.h"


/**
 * Routine which draw a text object
 * @param pObj the pointer on the text object
 */
int drawTextEntity( sciPointObj * pObj ) ;

/**
 * Given a matrix of strings and an array of cells positions, draw each strings in
 * the corresponding cell. The array of cells must not been turned, this function
 * manage the rotation itself. The alignment is also respected.
 * @param strings     set of strings to display.
 * @param cellsArray  position of the cells in which the strings are dispalyed.
 * @param center      position of the center of the string array around which the rotation
 *                    is done.
 * @param angle       rotation angle in radian of the displayed matrix around its
 *                    lower left point.
 * @param align       position of strings inside their cells.
 */
void drawStringsInPosition( StringMatrix     * strings    ,
                            Vect2iMatrix     * cellsArray ,
                            int                center[2]  ,
                            double             angle      ,
                            sciTextAlignment   align       ) ;

/**
 * Draw a string inside a box. The box can be rotated.
 * @param string displayed string
 * @param blCorner  Bottom left corner of the string.
 * @param urCorner  Upper right corner of the string.
 * @param rotCenter Rotation center.
 * @param angle     Rotation angle in radian.
 * @param align     Position of the string inside the box.
 */
void drawStringInCell( char             * string      ,
                       int                blCorner[2] ,
                       int                urCorner[2] ,
                       int                rotCenter[2],
                       double             angle       ,
                       sciTextAlignment   align        ) ;

/**
 * Inside a cell, return the margin to be used for the display of a string inside a cell.
 * @param stringSize width and height of the string wich will be displayed.
 * @param blCorner   the bottom left corner of the cell. The cell must not be turned
 *                   (ie axes aligned).
 * @param urCorner   the upper right corner of the cell.
 * @param align      specify the position of the string inside the cell.
 * @param lMargin    left margin (horizontal).
 * @param bMargin    bottom margin (vertical).

 */
void getStringMargins( int                stringSize[2],
                       int                blCorner[2]  ,
                       int                urCorner[2]  ,
                       sciTextAlignment   align        ,
                       int              * lMargin      ,
                       int              * bMargin       ) ;


#endif /* _DRAW_TEXT_HANDLE_H_ */
