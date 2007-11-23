/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizer.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing functions to synchronize the access to graphic */
/*        data                                                            */
/*------------------------------------------------------------------------*/

#include "GraphicSynchronizer.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
GraphicSynchronizer::GraphicSynchronizer( void )
{
  m_iNbDisplayers = 0;
  m_iNbReaders    = 0;
  m_iNbWriters    = 0;
}
/*---------------------------------------------------------------------------------*/
GraphicSynchronizer::~GraphicSynchronizer( void )
{

}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startWriting( void )
{
  enterCriticalSection();
  while ( !isWritable() )
  {
    // writer must be alone
    wait();
  }
  addWriter();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endWritting( void )
{
  enterCriticalSection();
  removeWriter();
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startReading( void )
{
  enterCriticalSection();
  while( !isReadable() )
  {
    // Thread can not read while someone is writing
    wait();
  }
  addReader();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endReading( void )
{
  enterCriticalSection();
  removeReader();
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::startDisplaying( void )
{
  enterCriticalSection();
  while ( !isDisplayable() )
  {
    // only one display at a time and can not display while writing
    wait();
  }
  addDisplayer();
  exitCriticalSection();

}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::endDisplaying( void )
{
  enterCriticalSection();
  removeDisplayer();
  notifyAll();
  exitCriticalSection();
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isWritable( void )
{
  return (m_iNbWriters == 0) && (m_iNbReaders == 0) && (m_iNbDisplayers == 0) ;
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isReadable( void )
{
  return (m_iNbWriters == 0) ;
}
/*---------------------------------------------------------------------------------*/
bool GraphicSynchronizer::isDisplayable( void )
{
  return (m_iNbWriters == 0) && (m_iNbDisplayers == 0) ;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addWriter( void )
{
  m_iNbWriters++;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeWriter( void )
{
  m_iNbWriters--;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addReader( void )
{
  m_iNbReaders++;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeReader( void )
{
  m_iNbReaders--;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::addDisplayer( void )
{
  m_iNbDisplayers++;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizer::removeDisplayer( void )
{
  m_iNbDisplayers--;
}
/*---------------------------------------------------------------------------------*/

}
