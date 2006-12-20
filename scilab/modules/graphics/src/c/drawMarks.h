/*------------------------------------------------------------------------*/
/* file: DrawMarks.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Routines to draw marks in the graphic window                    */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_MARKS_H_
#undef  _DRAW_MARKS_H_

#include "ObjectStructure.h"

/*--------------------------------------------------------------------------------------------------*/
int  DrawNewMarks( sciPointObj * pobj, int n1, int * xm, int * ym, int * DPI ) ;
void DrawMarks3D(  sciPointObj * pobj, int n1, int * xm, int * ym, int * DPI ) ;

/**
* draw the marks at the vertices of a polyline.
* @param[in] pObj Object on which the marks will be drawn. Might be a polyline, a segs or
*                 rectangle object.
* @param[in] nbMarks Number of marks to draw.
* @param[in] xCoord X coordinates of the marks in pixels.
* @param[in] yCoord Y coordinates of the marks in pixels.
* @param     DPI    ???
*/
void drawPolyMarks( sciPointObj * pObj, int nbMarks, int xCoord[], int yCoord[], int DPI[2] ) ;
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
int DrawMark_FullDot(           int xmi, int ymi, double size, int foreground, int background, int pixel_offset) ;
int DrawMark_Plus(              int xmi, int ymi, int size, int foreground, int pixel_offset) ;
int DrawMark_Cross(             int xmi, int ymi, int size, int foreground, int pixel_offset) ;
int DrawMark_FullDiamond(       int xmi, int ymi, int size, int foreground, int background) ;
int DrawMark_FullTriangleUp(    int xmi, int ymi, int size, int foreground, int background) ;
int DrawMark_FullTriangleDown(  int xmi, int ymi, int size, int foreground, int background) ;
int DrawMark_Asterisk(          int xmi, int ymi, int size, int foreground, int pixel_offset) ;
int DrawMark_FullSquare(        int xmi, int ymi, int size, int foreground, int background, int pixel_offset) ;
int DrawMark_FullTriangleRight( int xmi, int ymi, int size, int foreground, int background) ;
int DrawMark_FullTriangleLeft(  int xmi, int ymi, int size, int foreground, int background) ;
int DrawMark_FullPentagram(     int xmi, int ymi, int size, int foreground, int background) ;
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/**
* This function is used to determine how the pixel data
* is given to Xt method (like XDrawLines)
* 0 : pixel itself
* 1 : pixel's interval
* Explanation :
* i.e.in X11 (same thing using Win32 driver), with x2=x1+1 and y1=y2, we have :
*   XDrawLine(x1,y1,x1,y1)  does nothing (no pixel lit)
*   XDrawLine(x1,y1,x2,y2)  ONE pixel lit
* BUT the corresponding function with driver Gif (and others?) give us:
* for now (and only for marks), I make a trick using a pixel_offset set to 0 or 1
* depending on the driver.
* @author Fabrice Leray
*/
int CheckPixelStatus( void ) ;
/*--------------------------------------------------------------------------------------------------*/

#endif /* _DRAW_MARKS_H_ */
