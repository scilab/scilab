/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing functions to synchronize the access to graphic data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GraphicSynchronizer.h"

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
GraphicSynchronizer::GraphicSynchronizer( void )
{
  m_oWritersIds.clear();
  m_oReadersIds.clear();
  m_oDisplayersIds.clear();
  m_bIsEnable = true;
}
/*---------------------------------------------------------------------------------*/
GraphicSynchronizer::~GraphicSynchronizer( void )
{
  m_oWritersIds.clear();
  m_oReadersIds.clear();
  m_oDisplayersIds.clear();
  m_bIsEnable = false;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startWriting( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  while ( !isWritable(threadId) )
  {
    // writer must be alone
    wait();
  }
  addWriter(threadId);
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endWritting( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  removeWriter(threadId);
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startReading( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  while( !isReadable(threadId) )
  {
    // Thread can not read while someone is writing
    wait();
  }
  addReader(threadId);
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endReading( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  removeReader(threadId);
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startDisplaying( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  while ( !isDisplayable(threadId) )
  {
    // only one display at a time and can not display while writing
    wait();
  }
  addDisplayer(threadId);
  exitCriticalSection();

}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endDisplaying( void )
{
  int threadId = getCurrentThreadId();
  enterCriticalSection();
  removeDisplayer(threadId);
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::setEnable(bool enable)
{
  enterCriticalSection();
  m_bIsEnable = enable;
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isWritable( int threadId )
{
  // if not enable synchronizer is considered as able to write
  return (!m_bIsEnable) || (isOnlyWriter(threadId) && isOnlyDisplayer(threadId) && isOnlyReader(threadId));
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isReadable( int threadId )
{
  return (!m_bIsEnable) || isOnlyWriter(threadId);
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isDisplayable( int threadId )
{
  return (!m_bIsEnable) || (isOnlyWriter(threadId) && isOnlyDisplayer(threadId));
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addWriter( int threadId )
{
  m_oWritersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeWriter( int threadId )
{
  removeOne(m_oWritersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addReader( int threadId )
{
  m_oReadersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeReader( int threadId )
{
  removeOne(m_oReadersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addDisplayer( int threadId )
{
  m_oDisplayersIds.push_back(threadId);
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeDisplayer( int threadId )
{
  removeOne(m_oDisplayersIds, threadId);
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isOnlyWriter(int threadId)
{
  list<int>::iterator it = m_oWritersIds.begin();
  for ( ; it != m_oWritersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return true;
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isOnlyDisplayer(int threadId)
{
  list<int>::iterator it = m_oDisplayersIds.begin();
  for ( ; it != m_oDisplayersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return true;
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isOnlyReader(int threadId)
{
  list<int>::iterator it = m_oReadersIds.begin();
  for ( ; it != m_oReadersIds.end(); it++)
  {
    if (*it != threadId)
    {
      // there is an other thread
      return false;
    }
  }
  return true;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeOne(std::list<int>& intList, int value)
{
  list<int>::iterator it = intList.begin();
  for ( ;it != intList.end(); it++)
  {
    if (*it == value)
    {
      intList.erase(it);
      return;
    }
  }
}
/*---------------------------------------------------------------------------------*/
}
