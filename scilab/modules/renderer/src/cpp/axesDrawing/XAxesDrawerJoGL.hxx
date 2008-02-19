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

#ifndef _X_AXES_DRAWER_JOGL_HXX_
#define _X_AXES_DRAWER_JOGL_HXX_

#include "../subwinDrawing/TicksDrawerJoGL.hxx"
#include "XAxesDrawerJavaMapper.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class XAxesDrawerJoGL: public TicksDrawerJoGL
{
public:

  XAxesDrawerJoGL(DrawableAxes * axes);

  virtual ~XAxesDrawerJoGL(void);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  XAxesDrawerJavaMapper * getXAxesDrawerJavaMapper(void);

};

}

#endif /* _X_AXES_DRAWER_JOGL_HXX_ */

