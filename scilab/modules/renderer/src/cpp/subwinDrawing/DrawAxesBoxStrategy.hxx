/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for drawing the subwin surrounding parallelepiped 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_AXES_BOX_STRATEGY_HXX_
#define _DRAW_AXES_BOX_STRATEGY_HXX_

#include "DrawableSubwin.h"
#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

class DrawAxesBoxStrategy : public virtual DrawableObjectBridge
{
public:

  DrawAxesBoxStrategy(void) {}

  virtual ~DrawAxesBoxStrategy(void) {}

  /**
   * Main funtion which actually draw the box.
   * @param concealedCornerIndex index of the concealed corner
   */
  virtual void drawAxesBox(int concealedCornerIndex) = 0;

protected:
  

};

}

#endif /* _DRAW_AXES_BOX_STRATEGY_HXX_ */

