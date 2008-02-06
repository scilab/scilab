/*------------------------------------------------------------------------*/
/* file: FecFacetDrawerJavaMapper.hxx                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by FecFacetDrawer          */
/*------------------------------------------------------------------------*/

#ifndef _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_
#define _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "FecFacetDrawerGL.hxx"

namespace sciGraphics
{

class FecFacetDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  FecFacetDrawerJavaMapper(void);

  virtual ~FecFacetDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for fec facet line drawer
  virtual void setFacetParameters(double zMin, double zMax, int colMin,
                                  int colMax, int colOutLow, int colOutUp);

  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_fecDrawing::FecFacetDrawerGL * m_pJavaObject;


};

}

#endif /* _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_ */
