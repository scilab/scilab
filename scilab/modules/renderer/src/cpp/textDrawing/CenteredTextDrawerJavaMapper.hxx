/*------------------------------------------------------------------------*/
/* file: CenteredTextDrawerJavaMapper.hxx                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java method used by CenteredTextDrawerJoGL  */
/*------------------------------------------------------------------------*/

#ifndef _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_
#define _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_

#include "TextContentDrawerJavaMapper.hxx"
#include "CenteredTextDrawerGL.hxx"

namespace sciGraphics
{

class CenteredTextDrawerJavaMapper : public virtual TextContentDrawerJavaMapper
{

public:

  CenteredTextDrawerJavaMapper(void);

  virtual ~CenteredTextDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // Inherited from TextContentDrawerJavaMapper
  virtual void setTextContent(const char text[], int nbRow, int nbCol);
  virtual void setCenterPosition(double centerX, double centerY, double centerZ);
  virtual void drawTextContent(void);

  virtual double * getBoundingRectangle(void);

  virtual long * getScreenBoundingBox(void);
  /*----------------------------------------------------------------------*/
  // specific for centered text drawer
  virtual void setTextParameters(int textAlignment, int color, int fontStyle,
                                 double fontSize, double rotationAngle,
                                 int boxWidth, int boxHeight);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_textDrawing::CenteredTextDrawerGL * m_pJavaObject;


};

}

#endif /* _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_ */
