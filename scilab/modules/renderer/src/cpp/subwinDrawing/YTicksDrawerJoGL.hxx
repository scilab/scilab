/*------------------------------------------------------------------------*/
/* file: YTicksDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for Y axis                   */
/*------------------------------------------------------------------------*/

#ifndef _Y_TICKS_DRAWER_JOGL_HXX_
#define _Y_TICKS_DRAWER_JOGL_HXX_

#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJavaMapper.hxx"

namespace sciGraphics
{

class YTicksDrawerJoGL: public TicksDrawerJoGL
{
public:

  YTicksDrawerJoGL(DrawableSubwin * subwin);

  virtual ~YTicksDrawerJoGL(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  TicksDrawerJavaMapper * getYTicksDrawerJavaMapper(void);

};

}

#endif /* _Y_TICKS_DRAWER_JOGL_HXX_ */
