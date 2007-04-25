/*------------------------------------------------------------------------*/
/* file: DrawableSubwin.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a subwin object        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwin.h"
extern "C"
{
#include "handleDrawing/drawSubWinEntity.h"
}

/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::draw( void )
{
  drawSubWinEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
