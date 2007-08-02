/*------------------------------------------------------------------------*/
/* file: DrawableSubwinJavaMapper.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableSubwin          */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_
#define _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

/**
 * Contains mapping of java method used by DrawableSubwin 
 */
class DrawableSubwinJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  DrawableSubwinJavaMapper(void);

  virtual ~DrawableSubwinJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(void);

  virtual void destroy(void);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for subwins

  /*----------------------------------------------------------------------*/

protected:

};

}

#endif /* _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_ */