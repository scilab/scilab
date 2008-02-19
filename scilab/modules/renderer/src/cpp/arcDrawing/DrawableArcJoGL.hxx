/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw an
 * arc object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_ARC_JOGL_HXX_
#define _DRAWABLE_ARC_JOGL_HXX_

#include "DrawableArc.h"
#include "DrawableArcBridge.h"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{
  
class DrawableArcJoGL : public virtual DrawableArcBridge, public DrawableClippedObjectJoGL
{
public:

  DrawableArcJoGL(DrawableArc * arc) ;

  virtual ~DrawableArcJoGL(void);

  /**
   * Return the drawn object
   */
  virtual DrawableArc * getArcDrawer( void );

protected:


};

}

#endif /* _DRAWABLE_ARC_JOGL_HXX_ */

