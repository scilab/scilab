/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drivers independent routines to draw an
 * axes object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_AXES_HXX_
#define _CONCRETE_DRAWABLE_AXES_HXX_

#include "DrawableAxes.h"
#include "../subwinDrawing/TicksDrawer.hxx"

namespace sciGraphics
{

class ConcreteDrawableAxes : public DrawableAxes
{
public:

  ConcreteDrawableAxes(sciPointObj * pAxes);

  ~ConcreteDrawableAxes(void);

  void setTicksDrawer(TicksDrawer * ticksDrawer);

protected:

  /*---------------------------------------------------------------------*/
  /**
   * Actually draw the axes object
   */
  virtual void drawAxes(void);

  /**
   * Actually show the axes object
   */
  virtual void showAxes(void);
  /*---------------------------------------------------------------------*/
  TicksDrawer * m_pTicksDrawer;
  /*---------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_AXES_HXX_ */

