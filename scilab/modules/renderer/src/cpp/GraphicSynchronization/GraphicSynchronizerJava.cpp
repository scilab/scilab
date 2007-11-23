/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerJava.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Implementation of GraphicSynchronizer bridge using java         */
/*         methods                                                        */
/*------------------------------------------------------------------------*/

#include "GraphicSynchronizerJava.h"
extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
GraphicSynchronizerJava::GraphicSynchronizerJava( void )
{
  m_pJavaObject = new java_lang::Object(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
GraphicSynchronizerJava::~GraphicSynchronizerJava( void )
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::enterCriticalSection( void )
{
  m_pJavaObject->synchronize();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::exitCriticalSection( void )
{
  m_pJavaObject->endSynchronize();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::wait( void )
{
  m_pJavaObject->wait();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::notify( void )
{
  m_pJavaObject->notify();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::notifyAll( void )
{
  m_pJavaObject->notifyAll();
}
/*---------------------------------------------------------------------------------*/

}
