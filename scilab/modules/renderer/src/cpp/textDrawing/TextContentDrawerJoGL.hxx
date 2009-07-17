/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Abstract class for text drawing   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TEXT_CONTENT_DRAWER_JOGL_HXX_
#define _TEXT_CONTENT_DRAWER_JOGL_HXX_

#include "DrawableObjectJoGL.h"
#include "DrawTextContentStrategy.hxx"
#include "TextContentDrawerJavaMapper.hxx"

namespace sciGraphics
{

class TextContentDrawerJoGL : public DrawTextContentStrategy, public DrawableObjectJoGL
{
public:

  TextContentDrawerJoGL(DrawableText * drawer) ;

  virtual ~TextContentDrawerJoGL( void ) {}

  /**
   * Get the 4 corners of the text bounding rectangle (the text lies within a plane).
   * Used to draw the rectangle around the text.
   */
  virtual void getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Get the bounding box of the displayed text. Used to detect overlapping text.
   */
  virtual void getScreenBoundingBox(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Draw the rectangle surrounding the text using precomputed data
   */
  virtual void redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void);

protected:

  virtual void setDrawerParameters(void) = 0;

  /**
   * Convert distance in user mode to pixel.
   * Since it can be used with logarithmic mode, one edge of the segment must be specified.
   */
  void getPixelLength(sciPointObj * parentSubwin, const double startingPoint[3],
                      double userWidth, double userHeight,
                      double & pixelWidth, double & pixelHeight );

  /**
   * Get position of the text to display.
   */
  void getTextDisplayPos(double pos[3]);

  /**
   * Convert an array containing the coordinates of four corners
   * into 3 distincts vectors.
   */
  void convertCornersArray(const double corners[12],
                           double corner1[3],
                           double corner2[3],
                           double corner3[3],
                           double corner4[3]);

  /**
   * Compute the size in pixel of the user bounding box
   */
  void getUserSizePix(double & boxWidthPix, double & boxHeightPix);

  /**
   * Get the object performing mapping with Java class.
   */
  TextContentDrawerJavaMapper * getTextContentDrawerJavaMapper(void);

};

}

#endif /* _TEXT_CONTENT_DRAWER_JOGL_HXX_ */

