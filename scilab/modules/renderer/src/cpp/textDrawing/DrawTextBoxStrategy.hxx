/*------------------------------------------------------------------------*/
/* file: DrawTextBoxStrategy.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for drawing text box                                  */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_TEXT_BOX_STRATEGY_HXX_
#define _DRAW_TEXT_BOX_STRATEGY_HXX_

#include "DrawableText.h"

namespace sciGraphics
{
class DrawTextBoxStrategy
{
public:

  DrawTextBoxStrategy(DrawableText * text);

  virtual ~DrawTextBoxStrategy(void);

  /**
   * Specify the 4 corners of the text box.
   */
  void setBoxCorners(const double corner1[3], const double corner2[3],
                     const double corner3[3], const double corner4[3]);

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

  // The 4 corners of the box
  double m_aCorners[4][3];

};

}

#endif /* _DRAW_TEXT_BOX_STRATEGY_HXX_ */
