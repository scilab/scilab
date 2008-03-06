/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing implementation of driver independent  
 * algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_TEXT_H_
#define _CONCRETE_DRAWABLE_TEXT_H_

#include <list>

#include "DrawableText.h"
#include "DrawTextBoxStrategy.hxx"
#include "DrawTextContentStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableText : public DrawableText
{
public:

  ConcreteDrawableText(sciPointObj * pObj);

  virtual ~ConcreteDrawableText(void);

  /**
   * Get the 4 corners of the text bounding rectangle (the text lies within a plane).
   * Used to draw the rectangle around the text.
   */
  virtual void getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Get the bounding box of the displayed text. Used to detect overlapping text.
   */
  virtual void getScreenBoundingBox(int corner1[2], int corner2[2], int corner3[2], int corner4[2]);

  /**
   * Add a new strategy to draw the bounding rectangle of text.
   */
  void addBoxDrawingStrategy(DrawTextBoxStrategy * strategy);

  /**
   * Remove all strategies used to draw the bounding rectangle of text.
   */
  void removeBoxDrawingStrategies(void);

  /**
   * Set the strategy to use for drawing text.
   */
  void setTextDrawingStrategy(DrawTextContentStrategy * strategy);

  /**
   * To know if there is really some text to display.
   * Useful for labels objects which display text.
   */
  virtual bool isTextEmpty(void);

  /**
   * Update the text box of a text object
   */
  virtual void updateTextBox(void);

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawBox(void);

  /**
   * Draw the text of the object.
   */
  virtual void drawTextContent(void);

  /**
   * Display the rectangle surrounding the text using display lists.
   */
  virtual void showBox(void);

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void);
  /*---------------------------------------------------------------------------------*/
  DrawTextContentStrategy * m_pDrawingTextStrategy;
  std::list<DrawTextBoxStrategy *> m_oDrawingBoxStrategies;
  /*---------------------------------------------------------------------------------*/


};

}

#endif /* _CONCRETE_DRAWABLE_TEXT_H_ */

