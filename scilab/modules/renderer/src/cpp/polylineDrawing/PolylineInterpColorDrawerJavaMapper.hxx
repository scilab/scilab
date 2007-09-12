/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerJavaMapper.hxx                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by PolylineLineDrawer      */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_INTERP_COLOR_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_INTERP_COLOR_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
//#include "PolylineInterpColorDrawerGL.hxx"

namespace sciGraphics
{

class PolylineInterpColorDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineInterpColorDrawerJavaMapper(void);

  virtual ~PolylineInterpColorDrawerJavaMapper(void);

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

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            const int colors[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  //org_scilab_modules_renderer_polylineDrawing::PolylineInterpColorDrawerGL * m_pJavaObject;


};

}

#endif /* _POLYLINE_INTERP_COLOR_DRAWER_JAVA_MAPPER_HXX_ */
