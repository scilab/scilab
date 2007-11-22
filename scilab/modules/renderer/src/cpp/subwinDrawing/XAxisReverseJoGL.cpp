/*------------------------------------------------------------------------*/
/* file: XAxisReverseJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on X axis               */
/*------------------------------------------------------------------------*/

#include "XAxisReverseJoGL.hxx"

namespace sciGraphics
{
/*-----------------------------------------------------------------------------------*/
void XAxisReverseJoGL::revertAxis(void)
{
  m_pJavaMapper->revertXAxis();
}
/*-----------------------------------------------------------------------------------*/
}
