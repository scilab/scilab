/*------------------------------------------------------------------------*/
/* file: TextLineBoxDrawerJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the surrounding box a text object              */
/*------------------------------------------------------------------------*/

#ifndef _TEXT_LINE_BOX_DRAWER_JOGL_H_
#define _TEXT_LINE_BOX_DRAWER_JOGL_H_

#include "DrawTextBoxStrategy.hxx"
#include "../rectangleDrawing/RectangleLineDrawerJavaMapper.hxx"
#include "DrawableObjectJoGL.h"

namespace sciGraphics
{

class TextLineBoxDrawerJoGL : public DrawTextBoxStrategy, public DrawableObjectJoGL
{
public:

  TextLineBoxDrawerJoGL(DrawableText * text);

  virtual ~TextLineBoxDrawerJoGL(void);

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawBox(void);

  /**
   * Display the text using display lists.
   */
  virtual void showBox(void);

protected:

  /**
   * Rectangle drawer do the job.
   */
  RectangleLineDrawerJavaMapper * getLineDrawerJavaMapper(void);
  
};

}

#endif