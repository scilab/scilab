/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a text object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_TEXT_H_
#define _DRAWABLE_TEXT_H_


#include "DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableText : public DrawableClippedObject
{

public:

  DrawableText( sciPointObj * pObj );

  virtual ~DrawableText( void );

  /**
   * Get the 4 corners of the text bounding rectangle (the text lies within a plane).
   * Used to draw the rectangle around the text.
   */
  virtual void getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * Get the bounding box of the displayed text in pixels. Used to detect overlapping text.
   */
  virtual void getScreenBoundingBox(int corner1[2], int corner2[2], int corner3[2], int corner4[2]) = 0;

	/**
   * Get the bounding box of the displayed text in pixels with more precision.
   */
	virtual void getScreenBoundingBox(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * To know if there is really some text to display.
   * Useful for labels objects which display text.
   */
  virtual bool isTextEmpty(void) = 0;

  /**
   * Update the text box of a text object
   */
  virtual void updateTextBox(void) = 0;

  /**
   * Keep the default font size used by FilledTextDrawers
   */
  double getDefaultFontSize(void) {return m_dDefaultFontSize;}

  /**
   * Keep the default font size used by FilledTextDrawers
   */
  void setDefaultFontSize(double fontSize) {m_dDefaultFontSize = fontSize;}

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Arcs need to be redrawn each time the axes changes
   * since the view may have changed
   */
  virtual EDisplayStatus redraw(void) ;

  /**
   * Draw the text of the object.
   */
  virtual void drawTextContent(void) = 0;

  /**
   * Draw the text of the object using precomputed data
   */
  virtual void redrawTextContent(void) = 0;

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void) = 0;

  /*----------------------------------------------*/
  /**
   * Previous font size used by filled text drawer in Scilab Scale.
   * Might be negative for small text.
   */
  double m_dDefaultFontSize;
  /*----------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_TEXT_H_  */
