/*------------------------------------------------------------------------*/
/* file: ZTicksDrawerJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for Z axis                   */
/*------------------------------------------------------------------------*/

#ifndef _Z_TICKS_DRAWER_JOGL_HXX_
#define _Z_TICKS_DRAWER_JOGL_HXX_

#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class ZTicksDrawerJoGL: public TicksDrawerJoGL
{
public:

  ZTicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~ZTicksDrawerJoGL(void);

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  TicksDrawerJavaMapper * getZTicksDrawerJavaMapper(void);

};

}

#endif /* _Z_TICKS_DRAWER_JOGL_HXX_ */
