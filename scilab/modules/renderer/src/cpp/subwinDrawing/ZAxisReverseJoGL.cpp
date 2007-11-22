/*------------------------------------------------------------------------*/
/* file: ZAxisReverseJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for applying axes reverse mode on Z axis               */
/*------------------------------------------------------------------------*/

#include "ZAxisReverseJoGL.hxx"

namespace sciGraphics
{
/*-----------------------------------------------------------------------------------*/
void ZAxisReverseJoGL::revertAxis(void)
{
  m_pJavaMapper->revertZAxis();
}
/*-----------------------------------------------------------------------------------*/
}
