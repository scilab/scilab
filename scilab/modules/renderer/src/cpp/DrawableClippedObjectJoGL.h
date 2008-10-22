/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class with driver dependent algorithms of DrawableClippedObject
 * class using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_CLIPPED_OBJECT_JOGL_H_
#define _DRAWABLE_CLIPPED_OBJECT_JOGL_H_

#include "DrawableClippedObjectBridge.h"
#include "DrawableClippedObject.h"
#include "DrawableObjectJoGL.h"
#include "DrawableClippedObjectJavaMapper.hxx"

namespace sciGraphics
{

class DrawableClippedObjectJoGL : public virtual DrawableClippedObjectBridge, public DrawableObjectJoGL
{

public:

  DrawableClippedObjectJoGL( DrawableObject * drawer ) ;

  virtual ~DrawableClippedObjectJoGL( void ) {}


  /**
   * Set the clipping zone of the object.
   * Warning, be sure that min bounds are lower than max bounds.
   */
  virtual void setClipBox( double xMin, double xMax,
                           double yMin, double yMax,
                           double zMin, double zMax ) ;

  /**
   * Actually clip the object along each axis
   */
  virtual void clipX( void ) ;
  /**
   * Actually clip the object along each axis
   */
  virtual void clipY( void ) ;
  /**
   * Actually clip the object along each axis
   */
  virtual void clipZ( void ) ;

  /**
   * Remove clipping. Should be called at the end of the object drawing.
   * Otherwise it may have effect on other objects.
   */
  virtual void unClip( void ) ;

  /**
   * Return the drawed object
   */
  DrawableClippedObject * getClippedObjectDrawer( void ) ;


protected:

  /**
  * Get the object performing mapping with Java class.
  */
  DrawableClippedObjectJavaMapper * getClippedJavaMapper(void);

  double m_dXmin ;
  double m_dXmax ;

  double m_dYmin ;
  double m_dYmax ;

  double m_dZmin ;
  double m_dZmax ;

};

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_JOGL_H_ */
