/*------------------------------------------------------------------------*/
/* file: DrawableFigure.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a figure object        */
/*------------------------------------------------------------------------*/

#include "DrawableFigure.h"
#include "DrawableFigureImp.h"
extern "C"
{
#include "drawFigureEntity.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigure::DrawableFigure( sciPointObj * pObj )
  : DrawableObject( pObj )
{
  m_bNeedRedraw = false ; // no display lists for figure
  m_pSynchronizer = NULL;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
  if ( m_pSynchronizer != NULL )
  {
    delete m_pSynchronizer;
    m_pSynchronizer = NULL;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setColorMap( const double rgbMat[], int nbColor )
{
  getFigureImp()->setColorMap( rgbMat, nbColor ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::getColorMap( double rgbMat[] )
{
  getFigureImp()->getColorMap( rgbMat ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::getSize( int size[2] )
{
  getFigureImp()->getSize(size) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setSize( const int size[2] )
{
  getFigureImp()->setSize(size) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::getWindowSize( int size[2] )
{
  getFigureImp()->getWindowSize(size) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setWindowSize( const int size[2] )
{
  getFigureImp()->setWindowSize(size);
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::openRenderingCanvas( void )
{
  getFigureImp()->openRenderingCanvas( sciGetNum( m_pDrawed ) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::closeRenderingCanvas( void )
{
  getFigureImp()->closeRenderingCanvas() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::drawInContext( void )
{
  initializeDrawing() ;
  setBackgroundColor() ;
  if ( checkVisibility() )
  {
    displayChildren() ;
  }

  endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::draw( void )
{ 
  if ( !checkAutoRedraw() )
  {
    return ;
  }

  // make sure the context is created
  drawCanvas() ;

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigureImp * DrawableFigure::getFigureImp( void )
{
  return dynamic_cast<DrawableFigureImp *>(m_pImp) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::drawCanvas( void )
{
  getFigureImp()->drawCanvas() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setInfoMessage( const char * infoMessage )
{
  getFigureImp()->setInfoMessage(infoMessage) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setBackgroundColor( void )
{
  getFigureImp()->setBackgroundColor( sciGetGraphicContext(getDrawedObject())->backgroundcolor ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::getWindowPosition( int pos[2] )
{
  getFigureImp()->getWindowPosition(pos) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setWindowPosition( const int pos[2] )
{
  getFigureImp()->setWindowPosition(pos) ;
}
/*------------------------------------------------------------------------------------------*/

}
