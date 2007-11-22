/*------------------------------------------------------------------------*/
/* file: YAxisReverseJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on Y axis               */
/*------------------------------------------------------------------------*/

#ifndef _Y_AXIS_REVERSE_STRATEGY_JOGL_
#define _Y_AXIS_REVERSE_STRATEGY_JOGL_

#include "AxesReverseStrategy.hxx"

namespace sciGraphics
{

class YAxisReverseJoGL : public AxesReverseStrategy
{
public:

  YAxisReverseJoGL(void) : AxesReverseStrategy() {}

  virtual ~YAxisReverseJoGL(void) {}

  /**
  * Main function, cause one axis to be reverted.
  */
  virtual void revertAxis(void);

};

}

#endif /* _Y_AXIS_REVERSE_STRATEGY_JOGL_ */
