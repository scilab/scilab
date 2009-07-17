/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Interface for drawing text box  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_TEXT_CONTENT_STRATEGY_HXX_
#define _DRAW_TEXT_CONTENT_STRATEGY_HXX_

#include "DrawableText.h"

namespace sciGraphics
{
class DrawTextContentStrategy
{
public:

  DrawTextContentStrategy(DrawableText * text) { m_pDrawed = text; }

  virtual ~DrawTextContentStrategy(void) {m_pDrawed = NULL;}

  /**
   * Get the 4 corners of the text bounding rectangle (the text lies within a plane).
   * Used to draw the rectangle around the text.
   */
  virtual void getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * Get the bounding box of the displayed text. Used to detect overlapping text.
   */
  virtual void getScreenBoundingBox(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * Draw the rectangle surrounding the text using precomputed data
   */
  virtual void redrawTextContent(double corner1[3], double corner2[3], double corner3[3], double corner4[3]) = 0;

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void) = 0;

protected:

  DrawableText * m_pDrawed;

};

}

#endif /* _DRAW_TEXT_CONTENT_STRATEGY_HXX_ */

