/*------------------------------------------------------------------------*/
/* file: GlobalSynchronizer.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Synchronizer able to protect the whole graphic data             */
/*------------------------------------------------------------------------*/

#include "GlobalSynchronizer.h"

extern "C"
{
#include "MALLOC.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
GlobalSynchronizer::GlobalSynchronizer( void ) : GraphicSynchronizer()
{
  m_pBridge = NULL;
  m_iNbLocalDisplayers = 0;
  m_iNbLocalReaders = 0;
  m_iNbLocalWriters = 0;
}
/*------------------------------------------------------------------------------------------*/
GlobalSynchronizer::~GlobalSynchronizer( void )
{
  delete m_pBridge;
  m_pBridge = NULL;
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataWritable( void )
{
  // check if there is no other global synchronizer
  return GraphicSynchronizer::isWritable();
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataReadable( void )
{
  return GraphicSynchronizer::isReadable();
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataDisplayable( void )
{
  return GraphicSynchronizer::isDisplayable();
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isWritable( void )
{
  return areDataWritable() && (m_iNbLocalWriters == 0) && (m_iNbLocalReaders == 0) && (m_iNbLocalDisplayers == 0);
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isReadable( void )
{
  return areDataReadable() && (m_iNbLocalWriters == 0);
}
/*------------------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isDisplayable( void )
{
  return areDataDisplayable() && (m_iNbLocalWriters == 0) && (m_iNbLocalDisplayers == 0);
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalWriter( void )
{
  m_iNbLocalWriters++;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalWriter( void )
{
  m_iNbLocalWriters--;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalReader( void )
{
  m_iNbLocalReaders++;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalReader( void )
{
  m_iNbLocalReaders--;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalDisplayer( void )
{
  m_iNbLocalDisplayers++;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalDisplayer( void )
{
  m_iNbLocalDisplayers--;
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::enterCriticalSection( void )
{
  m_pBridge->enterCriticalSection();
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::exitCriticalSection( void )
{
  m_pBridge->exitCriticalSection();
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::wait( void )
{
  m_pBridge->wait();
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::notify( void )
{
  m_pBridge->notify();
}
/*------------------------------------------------------------------------------------------*/
void GlobalSynchronizer::notifyAll( void )
{
  m_pBridge->notifyAll();
}
/*------------------------------------------------------------------------------------------*/

}
