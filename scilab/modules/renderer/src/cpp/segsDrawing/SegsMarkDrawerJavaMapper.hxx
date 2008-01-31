/*------------------------------------------------------------------------*/
/* file: SegsMarkDrawerJavaMapper.hxx                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SegsMarkDrawer          */
/*------------------------------------------------------------------------*/

#ifndef _SEGS_MARK_DRAWER_JAVA_MAPPER_HXX_
#define _SEGS_MARK_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SegsMarkDrawerGL.hxx"

namespace sciGraphics
{

class SegsMarkDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  SegsMarkDrawerJavaMapper(void);

  virtual ~SegsMarkDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for segs mark drawer
  virtual void setMarkParameters(int background, int foreground, int markSizeUnit,
                                 int markSize, int markStyleIndex);

  virtual void drawSegs(const double startXCoords[], const double endXCoords[],
                        const double startYCoords[], const double endYCoords[],
                        const double startZCoords[], const double endZCoords[],
                        int nbSegments);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_segsDrawing::SegsMarkDrawerGL * m_pJavaObject;


};

}

#endif /* _SEGS_MARK_DRAWER_JAVA_MAPPER_HXX_ */
