/*------------------------------------------------------------------------*/
/* file: FecLineDrawerJavaMapper.hxx                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by FecLineDrawer          */
/*------------------------------------------------------------------------*/

#ifndef _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "FecLineDrawerGL.hxx"

namespace sciGraphics
{

class FecLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  FecLineDrawerJavaMapper(void);

  virtual ~FecLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for fec line line drawer
  virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_fecDrawing::FecLineDrawerGL * m_pJavaObject;


};

}

#endif /* _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_ */
