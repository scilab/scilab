/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerJavaMapper.h                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Implementation of GraphicSynchronizer bridge using java         */
/*         methods                                                        */
/*------------------------------------------------------------------------*/

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
