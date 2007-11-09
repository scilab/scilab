/*------------------------------------------------------------------------*/
/* file: TextFillBoxDrawerJoGL.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling the surrounding box a text object              */
/*------------------------------------------------------------------------*/

#ifndef _TEXT_FILL_BOX_DRAWER_JOGL_H_
#define _TEXT_FILL_BOX_DRAWER_JOGL_H_

#include "DrawTextBoxStrategy.hxx"
#include "../rectangleDrawing/RectangleFillDrawerJavaMapper.hxx"
#include "DrawableObjectJoGL.h"

namespace sciGraphics
{

class TextFillBoxDrawerJoGL : public DrawTextBoxStrategy, public DrawableObjectJoGL
{
public:

  TextFillBoxDrawerJoGL(DrawableText * text);

  virtual ~TextFillBoxDrawerJoGL(void);

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
  RectangleFillDrawerJavaMapper * getFillDrawerJavaMapper(void);

};

}

#endif /* _TEXT_FILL_BOX_DRAWER_JOGL_H_ */
