/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class Drawing the text content of a text object with no text    */
/*        box option                                                      */
/*------------------------------------------------------------------------*/

#ifndef _STANDARD_TEXT_DAWER_JOGL_HXX_
#define _STANDARD_TEXT_DAWER_JOGL_HXX_

#include "DrawTextContentStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "StandardTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class StandardTextDrawerJoGL : public DrawTextContentStrategy, public DrawableObjectJoGL
{
public:

  StandardTextDrawerJoGL(DrawableText * text);

  virtual ~StandardTextDrawerJoGL(void);

  /**
   * Get the 4 corners of the text bounding rectangle (the text lies within a plane).
   * Used to draw the rectangle around the text.
   */
  virtual void getBoundingRectangle(double corner1[3], double corner2[3], double corner3[3], double corner4[3]);

  /**
   * Get the bounding box of the displayed text. Used to detect overlapping text.
   */
  virtual void getScreenBoundingBox(int * xPos, int * yPos, int * width, int * height);

  /**
   * Draw the rectangle surrounding the text.
   */
  virtual void drawTextContent(void);

  /**
   * Display the text using display lists.
   */
  virtual void showTextContent(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  StandardTextDrawerJavaMapper * getStandardTextDrawerJavaMapper(void);

};

}

#endif /* _STANDARD_TEXT_DAWER_JOGL_HXX_ */