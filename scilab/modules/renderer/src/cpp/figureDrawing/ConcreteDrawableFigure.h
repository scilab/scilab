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

  ConcreteDrawableFigure( sciPointObj * pObj );

  ~ConcreteDrawableFigure(void);

  /**
   * Like display but to display only a single object
   */
  virtual void drawSingleObj(sciPointObj * pObj);

protected:

  /*------------------------------------------------------------------------*/
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) ;

  /**
   * Check wether the whole figure and hierechy should be redrawn
   * or just a single object.
   */
  virtual bool isDisplayingSingleObject(void);

  /**
   * Display the single object
   */
  virtual void displaySingleObject(void);
  /*------------------------------------------------------------------------*/
  sciPointObj * m_pSingleObject;
  /*------------------------------------------------------------------------*/



} ;

}

#endif /* _CONCRETE_DRAWABLE_FIGURE_H_  */
