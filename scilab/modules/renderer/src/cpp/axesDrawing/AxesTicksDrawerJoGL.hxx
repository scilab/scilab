/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class drawing X axis object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _AXES_TICKS_DRAWER_JOGL_HXX_
#define _AXES_TICKS_DRAWER_JOGL_HXX_

#include "../subwinDrawing/TicksDrawerJoGL.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class AxesTicksDrawerJoGL: public TicksDrawerJoGL
{
public:

  AxesTicksDrawerJoGL(DrawableAxes * axes);

  virtual ~AxesTicksDrawerJoGL(void);

  /**
   * Specify initialize drawing in setting all parameters specific to Axes
   */
  virtual void initializeDrawing(void);

protected:

};

}

#endif /* _AXES_TICKS_DRAWER_JOGL_HXX_ */

