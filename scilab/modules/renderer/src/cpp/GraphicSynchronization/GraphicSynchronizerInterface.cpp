/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizerInterface.cpp                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface to use class GraphicSynchronizer from C code          */
/*------------------------------------------------------------------------*/

#include "GraphicSynchronizerInterface.h"
#include "GraphicSynchronizer.h"
#include "GraphicSynchronizerFactory.h"
#include "getHandleDrawer.h"

using namespace sciGraphics;

/*---------------------------------------------------------------------------------*/
void startFigureDataWriting(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->startWriting();
}
/*---------------------------------------------------------------------------------*/
void endFigureDataWriting(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->endWritting();
}
/*---------------------------------------------------------------------------------*/
void startFigureDataReading(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->startReading();
}
/*---------------------------------------------------------------------------------*/
void endFigureDataReading(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->endReading();
}
/*---------------------------------------------------------------------------------*/
void startFigureDataDisplaying(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->startDisplaying();
}
/*---------------------------------------------------------------------------------*/
void endFigureDataDisplaying(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->getSynchronizer()->endDisplaying();
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
  GraphicSynchronizerFactory::getGlobalSynchronizer()->startWriting();
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
