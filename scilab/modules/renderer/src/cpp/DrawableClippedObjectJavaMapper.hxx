/*------------------------------------------------------------------------*/
/* file: DrawableClippedObjectJavaMapper.hxx                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableClippedObject   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_
#define _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class DrawableClippedObjectJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  DrawableClippedObjectJavaMapper( void ) {}

  virtual ~DrawableClippedObjectJavaMapper( void ) {}

  virtual void clipX(double xMin, double xMax) = 0;
  virtual void clipY(double yMin, double yMax) = 0;
  virtual void clipZ(double zMin, double zMax) = 0;

  virtual void unClip(void) = 0;

};

}

#endif /* _DRAWABLE_CLIPPED_OBJECT_JAVA_MAPPER_HXX_ */
