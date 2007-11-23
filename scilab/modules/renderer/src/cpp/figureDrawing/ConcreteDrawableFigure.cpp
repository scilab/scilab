/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableFigure.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent algorithms to draw a    */
/*        figure handle                                                   */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableFigure.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::checkAutoRedraw( void )
{
  return ( pFIGURE_FEATURE(m_pDrawed)->auto_redraw == TRUE ) ;
}
/*---------------------------------------------------------------------------------*/

}
