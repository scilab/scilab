/*------------------------------------------------------------------------*/
/* file: SurfaceLineDrawerJavaMapper.hxx                                  */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SurfaceLineDrawerJoGL   */
/*------------------------------------------------------------------------*/

#ifndef _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SurfaceLineDrawerGL.hxx"

namespace sciGraphics
{

class SurfaceLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  SurfaceLineDrawerJavaMapper(void);

  virtual ~SurfaceLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for surface line drawer
  virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

  virtual void setSurfaceType(int typeOfSurface);

  virtual void drawSurface(double xCoords[], int sizeX,
                           double yCoords[], int sizeY,
                           double zCoords[], int sizeZ,
                           int nbFacets);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_surfaceDrawing::SurfaceLineDrawerGL * m_pJavaObject;


};

}

#endif /* _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_ */
