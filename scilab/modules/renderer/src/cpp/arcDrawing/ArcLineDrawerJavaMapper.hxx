/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerJavaMapper.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by ArcLineDrawer           */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "ArcLineDrawerGL.hxx"

namespace sciGraphics
{

class ArcLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  ArcLineDrawerJavaMapper(void);

  virtual ~ArcLineDrawerJavaMapper(void);

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

  virtual void drawArc( double centerX       , double centerY       , double centerZ       ,
                        double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
                        double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
                        double startAngle    , double endAngle );
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_arcDrawing::ArcLineDrawerGL * m_pJavaObject;


};

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
