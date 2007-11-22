/*------------------------------------------------------------------------*/
/* file: YAxisReverseJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on Y axis               */
/*------------------------------------------------------------------------*/

#include "YAxisReverseJoGL.hxx"

namespace sciGraphics
{
/*-----------------------------------------------------------------------------------*/
void YAxisReverseJoGL::revertAxis(void)
{
  m_pJavaMapper->revertYAxis();
}
/*-----------------------------------------------------------------------------------*/
}
