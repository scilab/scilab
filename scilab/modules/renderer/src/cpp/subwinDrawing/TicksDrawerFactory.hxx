/*------------------------------------------------------------------------*/
/* file: TicksDrawerFactory.hxx                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for TicksDrawer classes                                 */
/*------------------------------------------------------------------------*/

#ifndef _TICKS_DRAWER_FACTORY_HXX_ 
#define _TICKS_DRAWER_FACTORY_HXX_

#include "DrawableSubwin.h"
#include "TicksDrawer.hxx"

namespace sciGraphics
{

class TicksDrawerFactory
{
public:

  TicksDrawerFactory(DrawableSubwin * subwin);

  virtual ~TicksDrawerFactory(void);

  /**
   * create ticks drawer for X axis
   */
  TicksDrawer * createXTicksDrawer(void);

  /**
   * Create ticks drawer for Y axis.
   */
  TicksDrawer * createYTicksDrawer(void);

  /**
   * Create ticks drawer for Z axis.
   */
  TicksDrawer * createZTicksDrawer(void);


protected:

  /** To know properties of the axes handle */
  DrawableSubwin * m_pDrawer;

};

}

#endif /* _TICKS_DRAWER_FACTORY_HXX_ */
