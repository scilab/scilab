/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw an
 * Axes object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_AXES_JOGL_HXX_
#define _DRAWABLE_AXES_JOGL_HXX_

#include "DrawableAxes.h"
#include "DrawableAxesBridge.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableAxesJoGL : public virtual DrawableAxesBridge, public DrawableClippedObjectJoGL
{
public:

  DrawableAxesJoGL(DrawableAxes * Axes) ;

  virtual ~DrawableAxesJoGL(void);

  /**
   * Return the drawn object
   */
  virtual DrawableAxes * getAxesDrawer( void );

protected:


};

}

#endif /* _DRAWABLE_AXES_JOGL_HXX_ */

