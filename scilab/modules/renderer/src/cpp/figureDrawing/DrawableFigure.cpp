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
#include "../../../graphics/src/c/handleDrawing/drawFigureEntity.h"
#include "../../../graphics/includes/GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigure::DrawableFigure( sciPointObj * pObj )
  : DrawableObject( pObj )
{
  m_bNeedRedraw = false ; // no display lists for figure
}
/*------------------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
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
    updateInfoMessage() ;
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
  openRenderingCanvas() ;
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
void DrawableFigure::updateInfoMessage( void  )
{
  getFigureImp()->updateInfoMessage() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::setBackgroundColor( void )
{
  getFigureImp()->setBackgroundColor( sciGetGraphicContext(getDrawedObject())->backgroundcolor ) ;
}
/*------------------------------------------------------------------------------------------*/
}
