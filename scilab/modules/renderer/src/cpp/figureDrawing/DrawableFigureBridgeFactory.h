/*------------------------------------------------------------------------*/
/* file: DrawableFigureBridgeFactory.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFigureImp                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_BRIDGE_FACTORY_H_
#define _DRAWABLE_FIGURE_BRIDGE_FACTORY_H_


#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"

namespace sciGraphics
{

class DrawableFigureBridgeFactory
{

public:

  DrawableFigureBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableFigureBridgeFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableFigureBridge * create( void ) ;

  /**
   * Set the DrawableFigure wich needs algorithms
   */
  void setDrawedFigure( DrawableFigure * fig ) ;

protected:

  DrawableFigure * m_pDrawable ;
  
} ;

}

#endif /* _DRAWABLE_FIGURE_BRIDGE_FACTORY_H_  */
