/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJavaMapper.hxx                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java method used by PolylineBarDrawerJoGL   */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineBarDrawerGL.hxx"

namespace sciGraphics
{

class PolylineBarDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineBarDrawerJavaMapper(void);

  virtual ~PolylineBarDrawerJavaMapper(void);

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
  virtual void setBarParameters(int background, int foreground, float thickness,
                                int lineStyle);

  virtual void drawPolyline(const double topX[],
                            const double topY[],
                            const double topZ[],
                            const double height[],
                            const double left[],
                            const double right[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineBarDrawerGL * m_pJavaObject;


};

}

#endif /* _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_ */
