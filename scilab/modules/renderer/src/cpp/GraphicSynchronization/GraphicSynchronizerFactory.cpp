/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory to create GraphicSynchronizer objects 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
