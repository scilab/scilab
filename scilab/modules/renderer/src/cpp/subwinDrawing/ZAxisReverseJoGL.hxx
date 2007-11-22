/*------------------------------------------------------------------------*/
/* file: ZAxisReverseJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on Z axis               */
/*------------------------------------------------------------------------*/

#ifndef _Z_AXIS_REVERSE_STRATEGY_JOGL_
#define _Z_AXIS_REVERSE_STRATEGY_JOGL_

#include "AxesReverseStrategy.hxx"

namespace sciGraphics
{

class ZAxisReverseJoGL : public AxesReverseStrategy
{
public:

  ZAxisReverseJoGL(void) : AxesReverseStrategy() {}

  virtual ~ZAxisReverseJoGL(void) {}

  /**
  * Main function, cause one axis to be reverted.
  */
  virtual void revertAxis(void);

};

}

#endif /* _Z_AXIS_REVERSE_STRATEGY_JOGL_ */
