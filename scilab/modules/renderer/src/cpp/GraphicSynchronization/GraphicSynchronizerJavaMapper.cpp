/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Implementation of GraphicSynchronizer bridge using java methods
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GraphicSynchronizerJavaMapper.h"
extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
GraphicSynchronizerJavaMapper::GraphicSynchronizerJavaMapper( void )
{
  m_pJavaObject = new org_scilab_modules_renderer_utils_graphicSynchronization::GraphicSynchronizerJava(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
GraphicSynchronizerJavaMapper::~GraphicSynchronizerJavaMapper( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJavaMapper::enterCriticalSection( void )
{
  m_pJavaObject->synchronize();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJavaMapper::exitCriticalSection( void )
{
  m_pJavaObject->endSynchronize();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJavaMapper::wait( void )
{
  m_pJavaObject->wait();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJavaMapper::notify( void )
{
  m_pJavaObject->notify();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJavaMapper::notifyAll( void )
{
  m_pJavaObject->notifyAll();
}
/*---------------------------------------------------------------------------------*/
int GraphicSynchronizerJavaMapper::getCurrentThreadId(void)
{
  return m_pJavaObject->getCurrentThreadId();
}
/*---------------------------------------------------------------------------------*/

}
