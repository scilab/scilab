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
  m_pImp = NULL ;
}
/*------------------------------------------------------------------------------------------*/
DrawableFigure::~DrawableFigure( void )
{
  closeRenderingCanvas() ;
  if ( m_pImp != NULL )
  {
    delete m_pImp ;
    m_pImp = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::openRenderingCanvas( void )
{
  m_pImp->openRenderingCanvas( sciGetNum( m_pDrawed ) ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::closeRenderingCanvas( void )
{
  m_pImp->closeRenderingCanvas() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::draw( void )
{ 
  if ( !checkAutoRedraw() ) { return ; }

  openRenderingCanvas() ;
  initializeDrawing() ;
  
  if ( checkVisibility() )
  {
    updateInfoMessage() ;
    displayChildren() ;
  }

  endDrawing() ;

}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::initializeDrawing( void )
{
  m_pImp->initializeDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::endDrawing( void )
{
  m_pImp->endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::updateInfoMessage( void  )
{
  m_pImp->updateInfoMessage() ;
}
/*------------------------------------------------------------------------------------------*/

}
