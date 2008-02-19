/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a    
 * segs object   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SEGS_BRIDGE_
#define _DRAWABLE_SEGS_BRIDGE_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableSegs;

class DrawableSegsBridge : public virtual DrawableObjectBridge
{
public:

  DrawableSegsBridge( void ) {}

  virtual ~DrawableSegsBridge( void ) {}

  /**
   * Return the drawn object
   */
  virtual DrawableSegs * getSegsDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_SEGS_BRIDGE_ */

