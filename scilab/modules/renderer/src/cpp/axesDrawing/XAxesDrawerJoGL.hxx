/*------------------------------------------------------------------------*/
/* file: XAxesDrawerJoGL.hxx                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing X axis object                                     */
/*------------------------------------------------------------------------*/

#ifndef _X_AXES_DRAWER_JOGL_HXX_
#define _X_AXES_DRAWER_JOGL_HXX_

#include "../subwinDrawing/TicksDrawerJoGL.hxx"
#include "XAxesDrawerJavaMapper.hxx"
#include "DrawableAxes.h"

namespace sciGraphics
{

class XAxesDrawerJoGL: public TicksDrawerJoGL
{
public:

  XAxesDrawerJoGL(DrawableAxes * axes);

  virtual ~XAxesDrawerJoGL(void);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  XAxesDrawerJavaMapper * getXAxesDrawerJavaMapper(void);

};

}

#endif /* _X_AXES_DRAWER_JOGL_HXX_ */
