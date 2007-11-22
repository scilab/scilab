/*------------------------------------------------------------------------*/
/* file: AxesReverseStrategy.hxx                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on each axis            */
/*------------------------------------------------------------------------*/

#ifndef _AXES_REVERSE_STRATEGY_
#define _AXES_REVERSE_STRATEGY_

#include "CameraJavaMapper.hxx"

namespace sciGraphics
{

class CameraBridge;

class AxesReverseStrategy
{
public:

  AxesReverseStrategy(void) { m_pJavaMapper = NULL; }

  virtual ~AxesReverseStrategy(void) { m_pJavaMapper = NULL; }

  /**
   * Main function, cause one axis to be reverted.
   */
  virtual void revertAxis(void) = 0;

  /**
   * Set the camera java mapper to use.
   */
  void setJavaMapper(CameraJavaMapper * mapper) { m_pJavaMapper = mapper; }

protected:

  CameraJavaMapper * m_pJavaMapper;

};

}

#endif /* _AXES_REVERSE_STRATEGY_ */
