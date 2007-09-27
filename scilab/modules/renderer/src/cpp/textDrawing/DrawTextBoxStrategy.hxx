/*------------------------------------------------------------------------*/
/* file: DrawTextBoxStrategy.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for drawing text box                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_TEXT_BOX_STRATEGY_
#define _DRAW_TEXT_BOX_STRATEGY_

#include "DrawableText.h"

namespace sciGraphics
{
class DrawTextBoxStrategy
{
public:

  DrawTextBoxStrategy(DrawableText * text) { m_pDrawed = text; }

  virtual ~DrawTextBoxStrategy(void) {m_pDrawed = NULL;}

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawBox(void) = 0;

  /**
   * Display the text using display lists.
   */
  virtual void showBox(void) = 0;

protected:

  DrawableText * m_pDrawed;

};

}

#endif /* _DRAW_TEXT_BOX_STRATEGY_ */