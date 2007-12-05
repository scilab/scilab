/*------------------------------------------------------------------------*/
/* file: FullBoxDrawerJavaMapper.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by FullBoxDrawerJoGL       */
/*------------------------------------------------------------------------*/

#ifndef _FULL_BOX_DRAWER_JAVA_MAPPER_HXX_
#define _FULL_BOX_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"
#include "FullBoxDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by HalfBoxDrawerJoGL 
 */
class FullBoxDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  FullBoxDrawerJavaMapper(void);

  virtual ~FullBoxDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for FullBoxDrawer
  virtual void setBoxParameters(int hiddenAxisColor, int backgroundColor, int lineColor,
                                int lineStyle, float thickness);
  virtual void drawBox(double xMin, double xMax, double yMin,
                       double yMax, double zMin, double zMax);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::FullBoxDrawerGL * m_pJavaObject;

};

}

#endif /* _FULL_BOX_DRAWER_JAVA_MAPPER_HXX_ */
