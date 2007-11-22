/*------------------------------------------------------------------------*/
/* file: XAxisReverseJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on X axis               */
/*------------------------------------------------------------------------*/

#ifndef _X_AXIS_REVERSE_STRATEGY_JOGL_
#define _X_AXIS_REVERSE_STRATEGY_JOGL_

#include "AxesReverseStrategy.hxx"

namespace sciGraphics
{

class XAxisReverseJoGL : public AxesReverseStrategy
{
public:

  XAxisReverseJoGL(void) : AxesReverseStrategy() {}

  virtual ~XAxisReverseJoGL(void) {}

  /**
   * Main function, cause one axis to be reverted.
   */
  virtual void revertAxis(void);

};

}

#endif /* _X_AXIS_REVERSE_STRATEGY_JOGL_ */
