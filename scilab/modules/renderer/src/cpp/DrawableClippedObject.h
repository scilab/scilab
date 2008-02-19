/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class which represent an object which can be clipped 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_CLIPPED_OBJECT_H_
#define _DRAWABLE_CLIPPED_OBJECT_H_

#include "DrawableClippedObjectBridge.h"
#include "DrawableObject.h"

namespace sciGraphics
{

/**
 * Object dedicated to render a sciPointObj with clipping.
 */
class DrawableClippedObject : public DrawableObject
{

public:

  DrawableClippedObject( sciPointObj * drawed ) ;


  /**
   * To know wether the object is clipped or not.
   */
  bool isClipped( void ) { return m_bIsClipped ; }

  /**
   * Automatically set the clipping of an object.
   */
  void setClipBox( void ) ;

  /**
   * Set the clipping zone of the object.
   * Warning, be sure that min bounds are lower than max bounds.
   */
  void setClipBox( double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax ) ;

  /**
   * Actually clip the object
   */
  void clip( void ) ;

  /**
   * Remove clipping. Should be called at the end of the object drawing.
   * Otherwise it may have effect on other objects.
   */
  void unClip( void ) ;
  
  /**
   * Get the bridge object
   */
  DrawableClippedObjectBridge * getClippedObjBridge( void ) ;

protected:

  /** To know if clipping is active */
  bool m_bIsClipped ;

  /** To know if clipping along X axis is enable */
  bool m_bXClippingEnable;

  /** To know if clipping along Y axis is enable */
  bool m_bYClippingEnable;

  /** To know if clipping along Z axis is enable */
  bool m_bZClippingEnable;

} ;

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_H_  */
