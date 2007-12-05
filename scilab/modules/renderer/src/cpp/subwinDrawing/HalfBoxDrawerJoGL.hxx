/*------------------------------------------------------------------------*/
/* file: HalfBoxDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing half of a subwin object bounding box              */
/*------------------------------------------------------------------------*/

#ifndef _HALF_BOX_DRAWER_JOGL_HXX_
#define _HALF_BOX_DRAWER_JOGL_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "../DrawableObjectJoGL.h"
#include "HalfBoxDrawerJavaMapper.hxx"

namespace sciGraphics
{

class HalfBoxDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:

  HalfBoxDrawerJoGL(DrawableSubwin * subwin);

  virtual ~HalfBoxDrawerJoGL(void);

  /**
  * Main funtion which actually draw the box.
  */
  virtual void drawAxesBox(void);

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  HalfBoxDrawerJavaMapper * getHalfBoxDrawerJavaMapper(void);

};

}

#endif /* _HALF_BOX_DRAWER_JOGL_HXX_ */
