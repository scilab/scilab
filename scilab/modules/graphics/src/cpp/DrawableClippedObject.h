/*------------------------------------------------------------------------*/
/* file: DrawableClippedObject.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class wich represent an object wich can be clipped              */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_CLIPPED_OBJECT_H_
#define _DRAWABLE_CLIPPED_OBJECT_H_

extern "C"
{
#include "ObjectStructure.h"
}

#include "DrawableObject.h"

namespace sciGraphics
{

/**
 * Object dedicated to render a sciPointObj.
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
   * Set the clipping zone of the object.
   * Warning, be sure that min bounds are lower than max bounds.
   */
  void setClipBox( double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax ) ;

  /**
   * Actually clip the object
   */
  virtual void clip( void ) ;

  /**
   * remove clipping. Should be called at the end of the object drawing.
   * Otherwise it may have effect on other objects.
   */
  virtual void unClip( void ) ;
  

protected:

  double m_dXmin ;
  double m_dXmax ;

  double m_dYmin ;
  double m_dYmax ;

  double m_dZmin ;
  double m_dZmax ;

  bool m_bIsClipped ;

} ;

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_H_  */
