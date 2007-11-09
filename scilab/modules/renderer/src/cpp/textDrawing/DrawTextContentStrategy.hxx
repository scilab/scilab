/*------------------------------------------------------------------------*/
/* file: DrawTextContentStrategy.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for drawing text box                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_TEXT_CONTENT_STRATEGY_
#define _DRAW_TEXT_CONTENT_STRATEGY_

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
  virtual void getScreenBoundingBox(int * xPos, int * yPos, int * width, int * height) = 0;

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawTextContent(void) = 0;

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void) = 0;

protected:

  DrawableText * m_pDrawed;

};

}

#endif /* _DRAW_TEXT_CONTENT_STRATEGY_ */
