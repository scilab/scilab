/*------------------------------------------------------------------------*/
/* file: DrawableFigure.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a figure object        */
/*------------------------------------------------------------------------*/

#include "DrawableFigure.h"
extern "C"
{
#include "handleDrawing/drawFigureEntity.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableFigure::DrawableFigure( sciPointObj * pObj )
  : DrawableObject( pObj )
{

}
/*------------------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
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

}
