/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a  
 * subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SUBWIN_BRIDGE_H_
#define _DRAWABLE_SUBWIN_BRIDGE_H_

#include "../DrawableObject.h"
#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

// To avoid cyclic dependances. 
class DrawableSubwin;

class DrawableSubwinBridge : public virtual DrawableObjectBridge
{

public:

  DrawableSubwinBridge( void ) {}

  virtual ~DrawableSubwinBridge( void ) {}

protected:

  /**
   * Return the drawed object
   */
  virtual DrawableSubwin * getSubwinDrawer( void ) = 0 ;

} ;

}

#endif /* _DRAWABLE_SUBWIN_BRIDGE_H_  */
