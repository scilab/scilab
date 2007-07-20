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

/*------------------------------------------------------------------------------------------*/
LocalSynchronizer::LocalSynchronizer( void ) : GraphicSynchronizer()
{
  m_pParentSynchronizer = NULL;
}
/*------------------------------------------------------------------------------------------*/
LocalSynchronizer::~LocalSynchronizer( void )
{
  
}
/*------------------------------------------------------------------------------------------*/
bool LocalSynchronizer::isWritable( void )
{
  return GraphicSynchronizer::isWritable() && m_pParentSynchronizer->areDataWritable();
}
/*------------------------------------------------------------------------------------------*/
bool LocalSynchronizer::isReadable( void )
{
  return GraphicSynchronizer::isReadable() && m_pParentSynchronizer->areDataReadable();
}
/*------------------------------------------------------------------------------------------*/
bool LocalSynchronizer::isDisplayable( void )
{
  return GraphicSynchronizer::isDisplayable() && m_pParentSynchronizer->areDataDisplayable();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::addWriter( void )
{
  GraphicSynchronizer::addWriter();
  m_pParentSynchronizer->addLocalWriter();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::removeWriter( void )
{
  GraphicSynchronizer::removeWriter();
  m_pParentSynchronizer->removeLocalWriter();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::addReader( void )
{
  GraphicSynchronizer::addReader();
  m_pParentSynchronizer->addLocalReader();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::removeReader( void )
{
  GraphicSynchronizer::removeReader();
  m_pParentSynchronizer->removeLocalReader();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::addDisplayer( void )
{
  GraphicSynchronizer::addDisplayer();
  m_pParentSynchronizer->addLocalDisplayer();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::removeDisplayer( void )
{
  GraphicSynchronizer::removeDisplayer();
  m_pParentSynchronizer->removeLocalDisplayer();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::enterCriticalSection( void )
{
  m_pParentSynchronizer->enterCriticalSection();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::exitCriticalSection( void )
{
  m_pParentSynchronizer->exitCriticalSection();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::wait( void )
{
  m_pParentSynchronizer->wait();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::notify( void )
{
  m_pParentSynchronizer->notify();
}
/*------------------------------------------------------------------------------------------*/
void LocalSynchronizer::notifyAll( void )
{
  m_pParentSynchronizer->notifyAll();
}
/*------------------------------------------------------------------------------------------*/

}
