/*------------------------------------------------------------------------*/
/* file: SurfaceFacetDrawerJavaMapper.hxx                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SurfaceFacetDrawerJoGL  */
/*------------------------------------------------------------------------*/

#ifndef _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_
#define _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SurfaceFacetDrawerGL.hxx"

namespace sciGraphics
{

class SurfaceFacetDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  SurfaceFacetDrawerJavaMapper(void);

  virtual ~SurfaceFacetDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for surface facet drawer
  virtual void setDefaultColors(int defaultColor, int hiddenColor);

  virtual void setSurfaceType(int typeOfSurface, int colorFlag);

  virtual void drawSurface(double xCoords[], int sizeX,
                           double yCoords[], int sizeY,
                           double zCoords[], int sizeZ,
                           int nbFacets);

  virtual void setFacetsColors(double colors[], int colorsSize);

  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_surfaceDrawing::SurfaceFacetDrawerGL * m_pJavaObject;


};

}

#endif /* _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_ */
