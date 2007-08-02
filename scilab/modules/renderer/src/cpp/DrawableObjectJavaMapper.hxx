/*------------------------------------------------------------------------*/
/* file: DrawableObjectJavaMapper.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableObject          */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_
#define _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_

namespace sciGraphics
{

class DrawableObjectJavaMapper
{
public:
  
  DrawableObjectJavaMapper(void) {}

  virtual ~DrawableObjectJavaMapper(void) {}

  virtual void display(void) = 0;

  virtual void initializeDrawing(int figureIndex) = 0;

  virtual void endDrawing(void) = 0;

  virtual void show(void) = 0;

  virtual void destroy(void) = 0;

  virtual void setFigureIndex(int figureIndex) = 0;

};

}

#endif /* _DRAWABLE_OBJECT_JAVA_MAPPER_HXX_ */
