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

#include <stdio.h>

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
  printf("Destroying\n");
  if ( m_pImp != NULL )
  {
    delete m_pImp ;
    m_pImp = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableFigure::draw( void )
{
  //drawFigureEntity( m_pDrawed ) ;
  
  if ( !checkAutoRedraw() ) { return ; }

  initializeDrawing() ;

  if ( checkVisibility() )
  {
    updateInfoMessage() ;
    drawChildren() ;
  }

  endDrawing() ;

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
