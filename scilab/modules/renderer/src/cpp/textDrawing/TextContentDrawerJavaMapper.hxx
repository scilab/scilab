/*------------------------------------------------------------------------*/
/* file: TextContentDrawerJavaMapper.hxx                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by TextContentDrawerJoGL   */
/*------------------------------------------------------------------------*/

#ifndef _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_
#define _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class TextContentDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  TextContentDrawerJavaMapper( void ) {}

  virtual ~TextContentDrawerJavaMapper( void ) {}

  virtual void setTextContent(char ** text, int nbRow, int nbCol) = 0;
  virtual void setCenterPosition(double centerX, double centerY, double centerZ) = 0;
  virtual void drawTextContent(void) = 0;

  virtual double * getBoundingRectangle(void) = 0;

  virtual long * getScreenBoundingBox(void) = 0;

};

}

#endif /* _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_ */
