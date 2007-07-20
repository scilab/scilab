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
#include "JniUtils.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
GraphicSynchronizerJava::GraphicSynchronizerJava( void )
  : DrawableObjectJoGL(NULL, "java/lang/Object")
{
  
}
/*------------------------------------------------------------------------------------------*/
GraphicSynchronizerJava::~GraphicSynchronizerJava( void )
{

}
/*------------------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::enterCriticalSection( void )
{
  jniUpdateCurrentEnv()->MonitorEnter(m_oDrawableObject);
}
/*------------------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::exitCriticalSection( void )
{
  jniUpdateCurrentEnv()->MonitorExit(m_oDrawableObject);
}
/*------------------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::wait( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "wait", "()V");
}
/*------------------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::notify( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "notify", "()V");
}
/*------------------------------------------------------------------------------------------*/
void GraphicSynchronizerJava::notifyAll( void )
{
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "notifyAll", "()V");
}
/*------------------------------------------------------------------------------------------*/

}