/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class with driver dependent algorithms of DrawableClippedObject class
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_CLIPPED_OBJECT_BRIDGE_H_
#define _DRAWABLE_CLIPPED_OBJECT_BRIDGE_H_

#include "DrawableObject.h"
#include "DrawableObjectBridge.h"

namespace sciGraphics
{

class DrawableClippedObject;

class DrawableClippedObjectBridge : public virtual DrawableObjectBridge
{
public:

  DrawableClippedObjectBridge( void ) {}

  virtual ~DrawableClippedObjectBridge( void ) {}

  /**
   * Set the clipping zone of the object.
   * Warning, be sure that min bounds are lower than max bounds.
   */
  virtual void setClipBox( double xMin, double xMax,
                           double yMin, double yMax,
                           double zMin, double zMax ) = 0 ;

  /**
   * Actually clip the object along each axis
   */
  virtual void clipX( void ) = 0 ;
  /**
   * Actually clip the object along each axis
   */
  virtual void clipY( void ) = 0 ;
  /**
   * Actually clip the object along each axis
   */
  virtual void clipZ( void ) = 0 ;

  /**
   * Remove clipping. Should be called at the end of the object drawing.
   * Otherwise it may have effect on other objects.
   */
  virtual void unClip( void ) = 0 ;

  /**
   * Return the drawed object
   */
  virtual DrawableClippedObject * getClippedObjectDrawer( void ) = 0 ;

protected:

};

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_BRIDGE_H_ */
