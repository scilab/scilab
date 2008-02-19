/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Synchronizer able to protect the whole graphic data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GlobalSynchronizer.h"

extern "C"
{
#include "MALLOC.h"
}

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
GlobalSynchronizer::GlobalSynchronizer( void ) : GraphicSynchronizer()
{
  m_pBridge = NULL;
  m_oLocalWritersIds.clear();
  m_oLocalDisplayersIds.clear();
  m_oLocalReadersIds.clear();
}
/*---------------------------------------------------------------------------------*/
GlobalSynchronizer::~GlobalSynchronizer( void )
{
  delete m_pBridge;
  m_pBridge = NULL;
  m_oLocalWritersIds.clear();
  m_oLocalDisplayersIds.clear();
  m_oLocalReadersIds.clear();
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataWritable( int threadId )
{
  // check if there is no other global synchronizer
  return GraphicSynchronizer::isWritable(threadId);
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataReadable( int threadId )
{
  return GraphicSynchronizer::isReadable(threadId);
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::areDataDisplayable( int threadId )
{
  return GraphicSynchronizer::isDisplayable(threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalWriter( int threadId )
{
  m_oLocalWritersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalWriter( int threadId )
{
  removeOne(m_oLocalWritersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalReader( int threadId )
{
  m_oLocalReadersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalReader( int threadId )
{
  removeOne(m_oLocalReadersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::addLocalDisplayer( int threadId )
{
  m_oLocalDisplayersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::removeLocalDisplayer( int threadId )
{
  removeOne(m_oLocalDisplayersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isOnlyWriter(int threadId)
{
  list<int>::iterator it = m_oLocalWritersIds.begin();
  for ( ; it != m_oLocalWritersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return GraphicSynchronizer::isOnlyWriter(threadId);
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isOnlyDisplayer(int threadId)
{
  list<int>::iterator it = m_oLocalDisplayersIds.begin();
  for ( ; it != m_oLocalDisplayersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return GraphicSynchronizer::isOnlyDisplayer(threadId);
}
/*---------------------------------------------------------------------------------*/
bool GlobalSynchronizer::isOnlyReader(int threadId)
{
  list<int>::iterator it = m_oLocalReadersIds.begin();
  for ( ; it != m_oLocalReadersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return GraphicSynchronizer::isOnlyReader(threadId);
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::enterCriticalSection( void )
{
  m_pBridge->enterCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::exitCriticalSection( void )
{
  m_pBridge->exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::wait( void )
{
  m_pBridge->wait();
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::notify( void )
{
  m_pBridge->notify();
}
/*---------------------------------------------------------------------------------*/
void GlobalSynchronizer::notifyAll( void )
{
  m_pBridge->notifyAll();
}
/*---------------------------------------------------------------------------------*/
int GlobalSynchronizer::getCurrentThreadId(void)
{
  return m_pBridge->getCurrentThreadId();
}
/*---------------------------------------------------------------------------------*/
}
