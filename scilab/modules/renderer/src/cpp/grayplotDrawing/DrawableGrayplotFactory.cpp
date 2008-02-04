/*------------------------------------------------------------------------*/
/* file: DrawableGRayplotFactory.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for grayplot objects. Automaticaly create a grayplot    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplotFactory.h"
#include "DrawableGrayplotBridgeFactory.hxx"
#include "GrayplotDecomposer.hxx"
#include "MatplotDecomposer.hxx"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableObject * DrawableGrayplotFactory::create( void )
{
  ConcreteDrawableGrayplot * newGray = new ConcreteDrawableGrayplot( m_pDrawed ) ;
  DrawableGrayplotBridgeFactory fact;
  fact.setDrawedGrayplot(newGray);
  newGray->setDrawableImp(fact.create());
  setStrategies(newGray);
  
  return newGray;
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotFactory::update( void )
{
  setStrategies(dynamic_cast<ConcreteDrawableGrayplot *>(getGrayplotDrawer(m_pDrawed)));
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotFactory::setStrategies(ConcreteDrawableGrayplot * grayplot)
{
  sciPointObj * pGray = grayplot->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);

  if (ppGray->type == 1 || ppGray->type == 2)
  {
     // matplot
    grayplot->setDeccompositionStrategy(new MatplotDecomposer(grayplot));
  }
  else
  {
    // grayplot
    grayplot->setDeccompositionStrategy(new GrayplotDecomposer(grayplot));
  }

}
/*---------------------------------------------------------------------------------*/
}
