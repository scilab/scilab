/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw an
 * Axes object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_AXES_BRIDGE_H_
#define _DRAWABLE_AXES_BRIDGE_H_

#include "DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableAxes ;

class DrawableAxesBridge : public virtual DrawableClippedObjectBridge
{
public:

  DrawableAxesBridge( void ) {}

  virtual ~DrawableAxesBridge( void ) {}

  /**
  * Return the drawn object
  */
  virtual DrawableAxes * getAxesDrawer( void ) = 0;

protected:


};

}

#endif /* _DRAWABLE_AXES_BRIDGE_H_ */

