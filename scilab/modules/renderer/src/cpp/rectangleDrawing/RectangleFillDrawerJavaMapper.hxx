/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJavaMapper.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by RectangleFillDrawer     */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class RectangleFillDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  RectangleFillDrawerJavaMapper(void);

  virtual ~RectangleFillDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(void);

  virtual void destroy(void);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle fill drawer
  virtual void setBackColor(int color);

  virtual void drawRectangle(double corner1X, double corner1Y, double corner1Z,
                             double corner2X, double corner2Y, double corner2Z,
                             double corner3X, double corner3Y, double corner3Z,
                             double corner4X, double corner4Y, double corner4Z);
  /*----------------------------------------------------------------------*/

protected:

};

}

#endif /* _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_ */