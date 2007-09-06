/*------------------------------------------------------------------------*/
/* file: PolylineMarkDrawerJavaMapper.hxx                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by PolylineMarkDrawer      */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_MARK_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_MARK_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineMarkDrawerGL.hxx"

namespace sciGraphics
{

class PolylineMarkDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineMarkDrawerJavaMapper(void);

  virtual ~PolylineMarkDrawerJavaMapper(void);

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
  virtual void setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineMarkDrawerGL * m_pJavaObject;


};

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
