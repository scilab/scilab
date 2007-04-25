/*------------------------------------------------------------------------*/
/* file: DrawableCompound.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a compound object      */
/*------------------------------------------------------------------------*/


#include "DrawableCompound.h"
extern "C"
{
#include "handleDrawing/drawCompoundEntity.h"
}

/*------------------------------------------------------------------------------------------*/
void DrawableCompound::draw( void )
{
  drawCompoundEntity( m_pDrawed ) ;
}
/*------------------------------------------------------------------------------------------*/
