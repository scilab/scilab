/*------------------------------------------------------------------------*/
/* file: DrawableFecJavaMapper.hxx                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableFec             */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_JAVA_MAPPER_HXX_
#define _DRAWABLE_FEC_JAVA_MAPPER_HXX_

#include "DrawableClippedObjectJavaMapper.hxx"
#include "DrawableFecGL.hxx"

namespace sciGraphics
{

class DrawableFecJavaMapper : public virtual DrawableClippedObjectJavaMapper
{
public:

  DrawableFecJavaMapper(void) ;

  virtual ~DrawableFecJavaMapper(void);

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
  // Specific to fec

  /*----------------------------------------------------------------------*/



private:

  /**
  * Giws generated wrapper
  */
  org_scilab_modules_renderer_fecDrawing::DrawableFecGL * m_pJavaObject;

};

}

#endif /* _DRAWABLE_FEC_JAVA_MAPPER_HXX_ */
