/*------------------------------------------------------------------------*/
/* file: DrawableSegsJavaMapper.hxx                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableSegs            */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_JAVA_MAPPER_HXX_
#define _DRAWABLE_SEGS_JAVA_MAPPER_HXX_

#include "DrawableClippedObjectJavaMapper.hxx"
#include "DrawableSegsGL.hxx"

namespace sciGraphics
{

class DrawableSegsJavaMapper : public virtual DrawableClippedObjectJavaMapper
{
public:

  DrawableSegsJavaMapper(void) ;

  virtual ~DrawableSegsJavaMapper(void);

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
  // Specific to rectangles

  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_segsDrawing::DrawableSegsGL * m_pJavaObject;

};

}

#endif /* _DRAWABLE_SEGS_JAVA_MAPPER_HXX_ */
