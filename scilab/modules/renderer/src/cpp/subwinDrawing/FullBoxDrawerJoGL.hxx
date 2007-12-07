/*------------------------------------------------------------------------*/
/* file: FullBoxDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing the full bounding box of a subwin object          */
/*------------------------------------------------------------------------*/

#ifndef _FULL_BOX_DRAWER_JOGL_HXX_
#define _FULL_BOX_DRAWER_JOGL_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "../DrawableObjectJoGL.h"
#include "FullBoxDrawerJavaMapper.hxx"

namespace sciGraphics
{

class FullBoxDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:

  FullBoxDrawerJoGL(DrawableSubwin * subwin);

  virtual ~FullBoxDrawerJoGL(void);

  /**
  * Main funtion which actually draw the box.
  */
  virtual void drawAxesBox(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  FullBoxDrawerJavaMapper * getFullBoxDrawerJavaMapper(void);

};

}

#endif /* _FULL_BOX_DRAWER_JOGL_HXX_ */
