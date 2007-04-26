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
  if ( m_pImp != NULL )
  {
    delete m_pImp ;
    m_pImp = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::draw( void )
{ 
  if ( !checkAutoRedraw() ) { return ; }

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
