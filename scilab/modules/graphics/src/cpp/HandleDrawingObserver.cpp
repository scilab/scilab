/*------------------------------------------------------------------------*/
/* file: HandleObserver.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Observer to automotically draw graphic handles                  */
/*------------------------------------------------------------------------*/


#include "HandleDrawingObserver.h"
#include "DrawingBridge.h"

/*------------------------------------------------------------------------------------------*/
void HandleDrawingObserver::update( void )
{
  sciDrawObj( m_pSubject ) ;
}
/*------------------------------------------------------------------------------------------*/
