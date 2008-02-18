/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableFigure.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent algorithms to draw a    */
/*        figure handle                                                   */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableFigure.h"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "pixel_mode.h"
#include "SetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::ConcreteDrawableFigure( sciPointObj * pObj )
    : DrawableFigure( pObj )
{
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFigure::~ConcreteDrawableFigure(void)
{
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::drawSingleObj(sciPointObj * pObj)
{
  // set the single object to draw
  m_pSingleObject = pObj;


  int curPixelMode = sciGetXorMode(m_pDrawed);

  // launch display
  // displaySingleObject will be called
  display();

  sciSetXorMode(m_pDrawed, curPixelMode);

  // restore normal mode
  m_pSingleObject = NULL;
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::checkAutoRedraw( void )
{
  return ( sciGetIsAutoDrawable(m_pDrawed) == TRUE ) ;
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableFigure::isDisplayingSingleObject(void)
{
  if (m_pSingleObject == NULL)
  {
    return false;
  }

  if (sciGetEntityType(m_pSingleObject) == SCI_FIGURE)
  {
    // should be itself so it is like a normal draw
    return false;
  }
    
  return true;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFigure::displaySingleObject(void)
{
  // tell the subwin to draw only this object
  getSubwinDrawer(sciGetParentSubwin(m_pSingleObject))->displaySingleObj(m_pSingleObject);
}
/*---------------------------------------------------------------------------------*/

}
