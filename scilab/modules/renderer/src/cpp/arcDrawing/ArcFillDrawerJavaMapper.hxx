/*------------------------------------------------------------------------*/
/* file: ArcFillDrawerJavaMapper.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by ArcFillDrawer           */
/*------------------------------------------------------------------------*/

#ifndef _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class ArcFillDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  ArcFillDrawerJavaMapper(void);

  virtual ~ArcFillDrawerJavaMapper(void);

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
  virtual void setBackColor(int color);

  virtual void drawArc( double centerX       , double centerY       , double centerZ       ,
    double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
    double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
    double startAngle    , double endAngle );
  /*----------------------------------------------------------------------*/

private:


};

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
