/*------------------------------------------------------------------------*/
/* file: BackTrihedronDrawerJavaMapper.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by BackTrihedronJoGL       */
/*------------------------------------------------------------------------*/

#ifndef _BACK_TRIHEDRON_JAVA_MAPPER_HXX_
#define _BACK_TRIHEDRON_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"
#include "BackTrihedronDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by BackTrihedronJoGL 
 */
class BackTrihedronDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  BackTrihedronDrawerJavaMapper(void);

  virtual ~BackTrihedronDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for back trihedron
  virtual void setBoxParameters(int hiddenAxisColor, int backgroundColor,
                                int lineStyle, float thickness);
  virtual void drawBox(double xMin, double xMax, double yMin,
                       double yMax, double zMin, double zMax);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::BackTrihedronDrawerGL * m_pJavaObject;

};

}

#endif /* _BACK_TRIHEDRON_JAVA_MAPPER_HXX_ */
