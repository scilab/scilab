/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw an arc object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_ARC_BRIDGE_H_
#define _DRAWABLE_ARC_BRIDGE_H_

#include "DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableArc ;

class DrawableArcBridge : public virtual DrawableClippedObjectBridge
{
public:

  DrawableArcBridge( void ) {}

  virtual ~DrawableArcBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawableArc * getArcDrawer( void ) = 0;

protected:


};

}

#endif /* _DRAWABLE_ARC_BRIDGE_H_ */
