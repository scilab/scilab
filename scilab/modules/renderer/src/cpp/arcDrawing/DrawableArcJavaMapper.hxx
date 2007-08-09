/*------------------------------------------------------------------------*/
/* file: DrawableArcJavaMapper.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java method used by DrawableArcJoGL         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_JAVA_MAPPER_HXX_
#define _DRAWABLE_ARC_JAVA_MAPPER_HXX_

#include "DrawableClippedObjectJavaMapper.hxx"

namespace sciGraphics
{

/**
 * Contains mapping of Java method used by DrawableArcJoGL 
 */
class DrawableArcJavaMapper : public virtual DrawableClippedObjectJavaMapper
{
public:

  DrawableArcJavaMapper(void);

  virtual ~DrawableArcJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // Inherited From DrawableClippedObjectJavaMapper
  virtual void clipX(double xMin, double xMax);
  virtual void clipY(double yMin, double yMax);
  virtual void clipZ(double zMin, double zMax);

  virtual void unClip(void);
  /*----------------------------------------------------------------------*/
  // Specific to arc

  /*----------------------------------------------------------------------*/


protected:

};

}

#endif /* _DRAWABLE_ARC_JAVA_MAPPER_HXX_ */