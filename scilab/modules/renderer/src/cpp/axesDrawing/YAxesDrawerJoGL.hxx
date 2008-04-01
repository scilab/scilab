/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class drawing Y axis object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Y_AXES_DRAWER_JOGL_HXX_
#define _Y_AXES_DRAWER_JOGL_HXX_

#include "../subwinDrawing/TicksDrawerJoGL.hxx"
#include "YAxesDrawerJavaMapper.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class YAxesDrawerJoGL: public TicksDrawerJoGL
{
public:

  YAxesDrawerJoGL(DrawableAxes * axes);

  virtual ~YAxesDrawerJoGL(void);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  YAxesDrawerJavaMapper * getYAxesDrawerJavaMapper(void);

};

}

#endif /* _Y_AXES_DRAWER_JOGL_HXX_ */

