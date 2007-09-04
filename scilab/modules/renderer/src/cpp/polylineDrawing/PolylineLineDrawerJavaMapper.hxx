/*------------------------------------------------------------------------*/
/* file: PolylineLineDrawerJavaMapper.hxx                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by PolylineLineDrawer      */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineLineDrawerGL.hxx"

namespace sciGraphics
{

class PolylineLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineLineDrawerJavaMapper(void);

  virtual ~PolylineLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for polyline line drawer
  virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineLineDrawerGL * m_pJavaObject;


};

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
