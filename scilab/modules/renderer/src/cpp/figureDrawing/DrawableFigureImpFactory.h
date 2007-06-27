/*------------------------------------------------------------------------*/
/* file: DrawableFigureImpFactory.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableFigureImp                                   */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_IMP_FACTORY_H_
#define _DRAWABLE_FIGURE_IMP_FACTORY_H_


#include "DrawableFigure.h"
#include "DrawableFigureImp.h"

namespace sciGraphics
{

class DrawableFigureImpFactory
{

public:

  DrawableFigureImpFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableFigureImpFactory( void ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableFigureImp * create( void ) ;

  /**
   * Set the DrawableFigure wich needs algorithms
   */
  void setDrawedFigure( DrawableFigure * fig ) ;

protected:

  DrawableFigure * m_pDrawable ;
  
} ;

}

#endif /* _DRAWABLE_FIGURE_IMP_FACTORY_H_  */
