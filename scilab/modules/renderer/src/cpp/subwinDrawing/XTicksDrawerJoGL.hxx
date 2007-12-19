/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#ifndef _X_TICKS_DRAWER_JOGL_HXX_
#define _X_TICKS_DRAWER_JOGL_HXX_

#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class XTicksDrawerJoGL: public TicksDrawerJoGL
{
public:

  XTicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~XTicksDrawerJoGL(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  TicksDrawerJavaMapper * getXTicksDrawerJavaMapper(void);

};

}

#endif /* _X_TICKS_DRAWER_JOGL_HXX_ */
