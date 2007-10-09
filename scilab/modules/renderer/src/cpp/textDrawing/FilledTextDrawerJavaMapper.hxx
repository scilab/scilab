/*------------------------------------------------------------------------*/
/* file: FilledTextDrawerJavaMapper.hxx                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by FilledTextDrawerJoGL    */
/*------------------------------------------------------------------------*/

#ifndef _FILLED_TEXT_DRAWER_JAVA_MAPPER_HXX_
#define _FILLED_TEXT_DRAWER_JAVA_MAPPER_HXX_

#include "TextContentDrawerJavaMapper.hxx"
#include "FilledTextDrawerGL.hxx"

namespace sciGraphics
{

class FilledTextDrawerJavaMapper : public virtual TextContentDrawerJavaMapper
{

public:

  FilledTextDrawerJavaMapper(void);

  virtual ~FilledTextDrawerJavaMapper(void);

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
  // specific for rectangle text drawer
  virtual void setTextParameters(int textAlignment, int color, int fontStyle,
                                 double rotationAngle, int boxWidth, int boxHeight);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_textDrawing::FilledTextDrawerGL * m_pJavaObject;


};

}

#endif /* _FILLED_TEXT_DRAWER_JAVA_MAPPER_HXX_ */
