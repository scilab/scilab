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
#include "TextContentDrawerJoGL.hxx"
#include "StandardTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class StandardTextDrawerJoGL : public TextContentDrawerJoGL
{
public:

  StandardTextDrawerJoGL(DrawableText * text);

  virtual ~StandardTextDrawerJoGL(void);

protected:

  virtual void setDrawerParameters(void);

  /**
   * Get the object performing mapping with Java class.
   */
  StandardTextDrawerJavaMapper * getStandardTextDrawerJavaMapper(void);

};

}

#endif /* _STANDARD_TEXT_DAWER_JOGL_HXX_ */