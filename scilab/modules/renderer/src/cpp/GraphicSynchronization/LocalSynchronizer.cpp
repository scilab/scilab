/*------------------------------------------------------------------------*/
/* file: LocalSynchronizer.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Synchronizer able to protect some part of the graphic data      */
/*------------------------------------------------------------------------*/

#include "LocalSynchronizer.h"

extern "C"
{
#include "MALLOC.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
LocalSynchronizer::LocalSynchronizer( void ) : GraphicSynchronizer()
{
  m_pParentSynchronizer = NULL;
}
/*---------------------------------------------------------------------------------*/
LocalSynchronizer::~LocalSynchronizer( void )
{
  
}
/*---------------------------------------------------------------------------------*/
bool LocalSynchronizer::isWritable( int threadId )
{
  return GraphicSynchronizer::isWritable(threadId) && m_pParentSynchronizer->areDataWritable(threadId);
}
/*---------------------------------------------------------------------------------*/
bool LocalSynchronizer::isReadable( int threadId )
{
  return GraphicSynchronizer::isReadable(threadId) && m_pParentSynchronizer->areDataReadable(threadId);
}
/*---------------------------------------------------------------------------------*/
bool LocalSynchronizer::isDisplayable( int threadId )
{
  return GraphicSynchronizer::isDisplayable(threadId) && m_pParentSynchronizer->areDataDisplayable(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::addWriter( int threadId )
{
  GraphicSynchronizer::addWriter(threadId);
  m_pParentSynchronizer->addLocalWriter(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::removeWriter( int threadId )
{
  GraphicSynchronizer::removeWriter(threadId);
  m_pParentSynchronizer->removeLocalWriter(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::addReader( int threadId )
{
  GraphicSynchronizer::addReader(threadId);
  m_pParentSynchronizer->addLocalReader(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::removeReader( int threadId )
{
  GraphicSynchronizer::removeReader(threadId);
  m_pParentSynchronizer->removeLocalReader(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::addDisplayer( int threadId )
{
  GraphicSynchronizer::addDisplayer(threadId);
  m_pParentSynchronizer->addLocalDisplayer(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::removeDisplayer( int threadId )
{
  GraphicSynchronizer::removeDisplayer(threadId);
  m_pParentSynchronizer->removeLocalDisplayer(threadId);
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::enterCriticalSection( void )
{
  m_pParentSynchronizer->enterCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::exitCriticalSection( void )
{
  m_pParentSynchronizer->exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::wait( void )
{
  m_pParentSynchronizer->wait();
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::notify( void )
{
  m_pParentSynchronizer->notify();
}
/*---------------------------------------------------------------------------------*/
void LocalSynchronizer::notifyAll( void )
{
  m_pParentSynchronizer->notifyAll();
}
/*---------------------------------------------------------------------------------*/
int LocalSynchronizer::getCurrentThreadId(void)
{
  return m_pParentSynchronizer->getCurrentThreadId();
}
/*---------------------------------------------------------------------------------*/
}
