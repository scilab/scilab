/*------------------------------------------------------------------------*/
/* file: DrawableFigureImp.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object                                                   */
/*------------------------------------------------------------------------*/


#include "DrawableFigureImp.h"
#include "DrawableFigure.h"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableFigureImp::DrawableFigureImp( DrawableFigure * drawer ) : DrawableObjectImp( drawer )
{

}
/*------------------------------------------------------------------------------------------*/
DrawableFigure * DrawableFigureImp::getDrawer( void )
{
  return dynamic_cast<DrawableFigure *>(m_pDrawer) ;
}
/*------------------------------------------------------------------------------------------*/
}
