/*------------------------------------------------------------------------*/
/* file: PolylineArrowDrawerJavaMapper.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java methods used by RectangleArrowDrawer   */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
//#include "RectangleArrowDrawerGL.hxx"

namespace sciGraphics
{

class PolylineArrowDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  PolylineArrowDrawerJavaMapper(void);

  virtual ~PolylineArrowDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for polyline arrow drawer
  virtual void setArrowParameters(int arrowColor, double arrowSize);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  //org_scilab_modules_renderer_polylineDrawing::polylineArrowDrawerGL * m_pJavaObject;


};

}

#endif /* _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_ */
