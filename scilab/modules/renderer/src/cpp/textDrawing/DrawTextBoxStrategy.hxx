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

