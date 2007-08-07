/*------------------------------------------------------------------------*/
/* file: RectangleLineDrawerJavaMapper.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by RectangleLineDrawer     */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "RectangleLineDrawerGL.hxx"

namespace sciGraphics
{

  class RectangleLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
  {

  public:

    RectangleLineDrawerJavaMapper(void);

    virtual ~RectangleLineDrawerJavaMapper(void);

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
    virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

    virtual void drawRectangle(double corner1X, double corner1Y, double corner1Z,
                               double corner2X, double corner2Y, double corner2Z,
                               double corner3X, double corner3Y, double corner3Z,
                               double corner4X, double corner4Y, double corner4Z);
    /*----------------------------------------------------------------------*/

  private:
  
    /**
     * Giws generated wrapper.
     */
    org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL * m_pJavaObject;
    

  };

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
