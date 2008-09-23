/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a
 * Fec objec
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FEC_BRIDGE_HXX_
#define _DRAWABLE_FEC_BRIDGE_HXX_

#include "DrawableObject.h"
#include "DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/* To avoid cyclic dependencies */
class DrawableFec;

class DrawableFecBridge : public virtual DrawableObjectBridge
{
public:

  DrawableFecBridge( void ) {}

  virtual ~DrawableFecBridge( void ) {}

  /**
  * Return the drawn object
  */
  virtual DrawableFec * getFecDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_FEC_BRIDGE_HXX_ */

