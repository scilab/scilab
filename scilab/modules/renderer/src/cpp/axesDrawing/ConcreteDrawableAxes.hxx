/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableAxes.h                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drivers independent routines to draw an    */
/*        axes object                                                     */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_AXES_HXX_
#define _CONCRETE_DRAWABLE_AXES_HXX_

#include "DrawableAxes.h"
#include "../subwinDrawing/TicksDrawer.hxx"

namespace sciGraphics
{

class ConcreteDrawableAxes : public DrawableAxes
{
public:

  ConcreteDrawableAxes(sciPointObj * pAxes);

  ~ConcreteDrawableAxes(void);

  void setTicksDrawer(TicksDrawer * ticksDrawer);

protected:

  /*---------------------------------------------------------------------*/
  /**
   * Actually draw the axes object
   */
  virtual void drawAxes(void);

  /**
   * Actually show the axes object
   */
  virtual void showAxes(void);
  /*---------------------------------------------------------------------*/
  TicksDrawer * m_pTicksDrawer;
  /*---------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_AXES_HXX_ */
