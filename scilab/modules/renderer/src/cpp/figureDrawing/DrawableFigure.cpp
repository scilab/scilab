/*------------------------------------------------------------------------*/
/* file: DrawableFigure.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a figure object        */
/*------------------------------------------------------------------------*/

#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"
extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFigure::DrawableFigure( sciPointObj * pObj )
  : DrawableObject( pObj )
{
  m_bNeedRedraw = false ; // no display lists for figure
  m_pSynchronizer = NULL;
}
/*---------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
  if ( m_pSynchronizer != NULL )
  {
    delete m_pSynchronizer;
    m_pSynchronizer = NULL;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setColorMap( const double rgbMat[], int nbColor )
{
  getFigureImp()->setColorMap( rgbMat, nbColor ) ;

  // we need to redraw every object to take new colors into account.
  familyHasChanged();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getColorMap( double rgbMat[] )
{
  getFigureImp()->getColorMap( rgbMat ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getSize( int size[2] )
{
  getFigureImp()->getSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setSize( const int size[2] )
{
  getFigureImp()->setSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getWindowSize( int size[2] )
{
  getFigureImp()->getWindowSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setWindowSize( const int size[2] )
{
  getFigureImp()->setWindowSize(size);
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::openRenderingCanvas( void )
{
  getFigureImp()->openRenderingCanvas( sciGetNum( m_pDrawed ) ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::closeRenderingCanvas( void )
{
  getFigureImp()->closeRenderingCanvas() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::drawInContext( void )
{
  initializeDrawing() ;
  setFigureParameters() ;
  // retest on auto_redraw
  // because display may come directly from JoGL
  if ( checkVisibility() && checkAutoRedraw() )
  {
    displayChildren() ;
  }
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::draw( void )
{ 
  if ( !checkAutoRedraw() )
  {
    return ;
  }

  // make sure the context is created
  drawCanvas() ;

}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::show( void )
{
  draw() ;
}
/*---------------------------------------------------------------------------------*/
DrawableFigureBridge * DrawableFigure::getFigureImp( void )
{
  return dynamic_cast<DrawableFigureBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::drawCanvas( void )
{
  getFigureImp()->drawCanvas() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setInfoMessage( const char * infoMessage )
{
  getFigureImp()->setInfoMessage(infoMessage) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setFigureParameters( void )
{
  getFigureImp()->setFigureParameters() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::getWindowPosition( int pos[2] )
{
  getFigureImp()->getWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setWindowPosition( const int pos[2] )
{
  getFigureImp()->setWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setPixmapMode(bool onOrOff)
{
  getFigureImp()->setPixmapMode(onOrOff);
}
/*---------------------------------------------------------------------------------*/
bool DrawableFigure::getPixmapMode(void)
{
  return getFigureImp()->getPixmapMode();
}
/*---------------------------------------------------------------------------------*/
void DrawableFigure::setRenderingEnable(bool isEnable)
{
  getFigureImp()->setRenderingEnable(isEnable);
}
/*---------------------------------------------------------------------------------*/

}
