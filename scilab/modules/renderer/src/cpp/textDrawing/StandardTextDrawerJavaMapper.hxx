/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerJavaMapper.hxx                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by StandardTextDrawerJoGL  */
/*------------------------------------------------------------------------*/

#ifndef _STANDARD_TEXT_DRAWER_JAVA_MAPPER_HXX_
#define _STANDARD_TEXT_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "StandardTextDrawerGL.hxx"

namespace sciGraphics
{

class StandardTextDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  StandardTextDrawerJavaMapper(void);

  virtual ~StandardTextDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle text drawer
  virtual void setTextParameters(int textAlignment, int color, int fontStyle, double fontSize);

  virtual void setTextContent(const char text[], int nbRow, int nbCol);

  virtual void drawTextContent(double centerX, double centerY, double centerZ);

  virtual double * getBoundingRectangle(void);

  virtual long * getScreenBoundingBox(void);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_textDrawing::StandardTextDrawerGL * m_pJavaObject;


};

}

#endif /* _STANDARD_TEXT_DRAWER_JAVA_MAPPER_HXX_ */
