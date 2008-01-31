/*------------------------------------------------------------------------*/
/* file: SegsArrowDrawerJavaMapper.hxx                                    */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SegsArrowDrawer         */
/*------------------------------------------------------------------------*/

#ifndef _SEGS_ARROW_DRAWER_JAVA_MAPPER_HXX_
#define _SEGS_ARROW_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SegsArrowDrawerGL.hxx"

namespace sciGraphics
{

class SegsArrowDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  SegsArrowDrawerJavaMapper(void);

  virtual ~SegsArrowDrawerJavaMapper(void);

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
  virtual void setArrowSize(double size);

  virtual void setArrowSizes(const double sizes[], int nbSegments);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void drawSegs(const double startXCoords[], const double endXCoords[],
                        const double startYCoords[], const double endYCoords[],
                        const double startZCoords[], const double endZCoords[],
                        const int colors[], int nbSegments);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_segsDrawing::SegsArrowDrawerGL * m_pJavaObject;


};

}

#endif /* _SEGS_ARROW_DRAWER_JAVA_MAPPER_HXX_ */
