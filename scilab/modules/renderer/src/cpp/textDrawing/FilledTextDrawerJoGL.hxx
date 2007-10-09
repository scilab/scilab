/*------------------------------------------------------------------------*/
/* file: FilledTextDrawerJoGL.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class Drawing the text content of a text object which is filled */
/*        inside a text box                                               */
/*------------------------------------------------------------------------*/

#ifndef _FILLED_TEXT_DAWER_JOGL_HXX_
#define _FILLED_TEXT_DAWER_JOGL_HXX_

#include "DrawTextContentStrategy.hxx"
#include "TextContentDrawerJoGL.hxx"
#include "FilledTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class FilledTextDrawerJoGL : public TextContentDrawerJoGL
{
public:

  FilledTextDrawerJoGL(DrawableText * text);

  virtual ~FilledTextDrawerJoGL(void);

protected:

  virtual void setDrawerParameters(void);

  /**
   * Get the object performing mapping with Java class.
   */
  FilledTextDrawerJavaMapper * getFilledTextDrawerJavaMapper(void);

};

}

#endif /* _FILLED_TEXT_DAWER_JOGL_HXX_ */
