/*------------------------------------------------------------------------*/
/* file: YAxesDrawerJoGL.hxx                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing Y axis object                                     */
/*------------------------------------------------------------------------*/

#ifndef _Y_AXES_DRAWER_JOGL_HXX_
#define _Y_AXES_DRAWER_JOGL_HXX_

#include "../subwinDrawing/TicksDrawerJoGL.hxx"
#include "YAxesDrawerJavaMapper.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class YAxesDrawerJoGL: public TicksDrawerJoGL
{
public:

  YAxesDrawerJoGL(DrawableAxes * axes);

  virtual ~YAxesDrawerJoGL(void);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  YAxesDrawerJavaMapper * getYAxesDrawerJavaMapper(void);

};

}

#endif /* _Y_AXES_DRAWER_JOGL_HXX_ */
