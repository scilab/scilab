/*------------------------------------------------------------------------*/
/* file: CenteredTextDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class Drawing the text content of a text object with centered   */
/*        text box mode option                                            */
/*------------------------------------------------------------------------*/

#ifndef _CENTERED_TEXT_DAWER_JOGL_HXX_
#define _CENTERED_TEXT_DAWER_JOGL_HXX_

#include "DrawTextContentStrategy.hxx"
#include "TextContentDrawerJoGL.hxx"
#include "CenteredTextDrawerJavaMapper.hxx"

namespace sciGraphics
{

class CenteredTextDrawerJoGL : public TextContentDrawerJoGL
{
public:

  CenteredTextDrawerJoGL(DrawableText * text);

  virtual ~CenteredTextDrawerJoGL(void);

protected:

  virtual void setDrawerParameters(void);

  /**
  * Get the object performing mapping with Java class.
  */
  CenteredTextDrawerJavaMapper * getCenteredTextDrawerJavaMapper(void);

};

}

#endif /* _STANDARD_TEXT_DAWER_JOGL_HXX_ */
