/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Interface to use class GraphicSynchronizer from C code
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GraphicSynchronizerInterface.h"
#include "GraphicSynchronizer.h"
#include "GraphicSynchronizerFactory.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "InitObjects.h"
}

using namespace sciGraphics;

/*---------------------------------------------------------------------------------*/
void startFigureDataWriting(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->startWriting();
  }
}
/*---------------------------------------------------------------------------------*/
void endFigureDataWriting(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->endWritting();
  }
}
/*---------------------------------------------------------------------------------*/
void startFigureDataReading(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->startReading();
  }
}
/*---------------------------------------------------------------------------------*/
void endFigureDataReading(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->endReading();
  }
}
/*---------------------------------------------------------------------------------*/
void startFigureDataDisplaying(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->startDisplaying();
  }
}
/*---------------------------------------------------------------------------------*/
void endFigureDataDisplaying(sciPointObj * pFigure)
{
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->endDisplaying();
  }
}
/*---------------------------------------------------------------------------------*/
void startGraphicDataWriting( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->startWriting();
}
/*---------------------------------------------------------------------------------*/
void endGraphicDataWriting( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->endWritting();
}
/*---------------------------------------------------------------------------------*/
void startGraphicDataReading( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->startReading();
}
/*---------------------------------------------------------------------------------*/
void endGraphicDataReading( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->endReading();
}
/*---------------------------------------------------------------------------------*/
void startGraphicDataDisplaying( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->startDisplaying();
}
/*---------------------------------------------------------------------------------*/
void endGraphicDataDisplaying( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->endDisplaying();
}
/*---------------------------------------------------------------------------------*/
void enableFigureSynchronization(sciPointObj * pFigure)
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->setEnable(true);
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->setEnable(true);
  }
}
/*---------------------------------------------------------------------------------*/
void disableFigureSynchronization(sciPointObj * pFigure)
{
  GraphicSynchronizerFactory::getGlobalSynchronizer()->setEnable(false);
  if (!isFigureModel(pFigure))
  {
    getFigureDrawer(pFigure)->getSynchronizer()->setEnable(false);
  }
}
/*---------------------------------------------------------------------------------*/
void createGraphicSynchronizer( void )
{
  GraphicSynchronizerFactory::getGlobalSynchronizer();
}
/*---------------------------------------------------------------------------------*/
void destroyGraphicSynchronizer( void )
{
  GraphicSynchronizerFactory::destroyGlobalSynchronizer();
}
/*---------------------------------------------------------------------------------*/
