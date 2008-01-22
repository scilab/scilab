/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableFigure.h                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent algorithms to draw a    */
/*        figure handle                                                   */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_FIGURE_H_
#define _CONCRETE_DRAWABLE_FIGURE_H_


#include "DrawableFigure.h"

namespace sciGraphics
{

class ConcreteDrawableFigure : public DrawableFigure
{

public:

  ConcreteDrawableFigure( sciPointObj * pObj ) : DrawableFigure( pObj ) {}

  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) ;

} ;

}

#endif /* _CONCRETE_DRAWABLE_FIGURE_H_  */
