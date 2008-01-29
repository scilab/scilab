/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJavaMapper.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by RectangleMarkDrawer     */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_MARK_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_MARK_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "RectangleMarkDrawerGL.hxx"

namespace sciGraphics
{

class RectangleMarkDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  RectangleMarkDrawerJavaMapper(void);

  virtual ~RectangleMarkDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle fill drawer
  virtual void setMarkParameters(int background, int foreground,
                                 int markSizeUnit, int markSize, int markStyleIndex);

  virtual void drawRectangle(double corner1X, double corner1Y, double corner1Z,
                             double corner2X, double corner2Y, double corner2Z,
                             double corner3X, double corner3Y, double corner3Z,
                             double corner4X, double corner4Y, double corner4Z);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL * m_pJavaObject;

};

}

#endif /* _RECTANGLE_MARK_DRAWER_JAVA_MAPPER_HXX_ */
