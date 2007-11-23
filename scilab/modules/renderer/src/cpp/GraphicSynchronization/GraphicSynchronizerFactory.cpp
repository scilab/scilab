/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerFactory.cpp                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory to create GraphicSynchronizer objects                   */
/*------------------------------------------------------------------------*/

#include "GraphicSynchronizerFactory.h"
#include "GraphicSynchronizerBridgeFactory.h"

extern "C"
{
#include "MALLOC.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
GlobalSynchronizer * GraphicSynchronizerFactory::s_pGlobalSynchronizer = NULL;
/*---------------------------------------------------------------------------------*/
GlobalSynchronizer * GraphicSynchronizerFactory::getGlobalSynchronizer( void )
{
  if (s_pGlobalSynchronizer == NULL)
  {
    GraphicSynchronizerFactory fact;
    s_pGlobalSynchronizer = fact.createGlobalSynchronizer();
  }
  return s_pGlobalSynchronizer;
}
/*---------------------------------------------------------------------------------*/
void GraphicSynchronizerFactory::destroyGlobalSynchronizer( void )
{
  if (s_pGlobalSynchronizer != NULL)
  {
    delete s_pGlobalSynchronizer;
    s_pGlobalSynchronizer = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
LocalSynchronizer * GraphicSynchronizerFactory::createLocalSynchronizer( void )
{
  // Create instance
  LocalSynchronizer * res = new LocalSynchronizer();
  res->setParentSynchronizer(getGlobalSynchronizer());

  return res;
}
/*---------------------------------------------------------------------------------*/
GlobalSynchronizer * GraphicSynchronizerFactory::createGlobalSynchronizer( void )
{
  GlobalSynchronizer * res = new GlobalSynchronizer();

  GraphicSynchronizerBridgeFactory fact;
  res->setGraphicSynchronizerImp(fact.create());

  return res;

}
/*---------------------------------------------------------------------------------*/

}
