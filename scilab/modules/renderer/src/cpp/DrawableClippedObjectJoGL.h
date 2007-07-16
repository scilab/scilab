/*------------------------------------------------------------------------*/
/* file: DrawableClippedObjectJoGL.h                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class with driver dependent algorithms of DrawableClippedObject */
/*        class using JoGL                                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_CLIPPED_OBJECT_JOGL_H_
#define _DRAWABLE_CLIPPED_OBJECT_JOGL_H_

#include "DrawableClippedObjectBridge.h"
#include "DrawableClippedObject.h"
#include "DrawableObjectJoGL.h"

namespace sciGraphics
{

class DrawableClippedObjectJoGL : public virtual DrawableClippedObjectBridge, public DrawableObjectJoGL
{

public:

  DrawableClippedObjectJoGL( DrawableObject * drawer, const char * className ) ;

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
  virtual DrawableClippedObject * getClippedObjectDrawer( void ) ;

protected:

  double m_dXmin ;
  double m_dXmax ;

  double m_dYmin ;
  double m_dYmax ;

  double m_dZmin ;
  double m_dZmax ;

};

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_JOGL_H_ */
