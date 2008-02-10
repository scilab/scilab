/*------------------------------------------------------------------------*/
/* file: TicksDrawer.hxx                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks                              */
/*------------------------------------------------------------------------*/

#ifndef _TICKS_DRAWER_HXX_ 
#define _TICKS_DRAWER_HXX_

#include "../DrawableObjectBridge.h"
#include "ComputeTicksStrategy.hxx"
#include "DrawableSubwin.h"
#include "GridDrawer.hxx"

namespace sciGraphics
{

class TicksDrawer : public virtual DrawableObjectBridge
{
public:
  
  TicksDrawer(DrawableObject * drawer);

  virtual ~TicksDrawer(void);

  void setTicksComputer(ComputeTicksStrategy * ticksComputer);

  void setGridDrawer(GridDrawer * gridDrawer);

  /**
   * Main function which draw ticks.
   */
  double draw(void);

  /**
   * Redefined show since we can not use display lists.
   */
  double showTicks(void);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Compute ticks positions and draw ticks from the computed position.
   */
  double drawTicks(void);

  /**
   * Draw the ticks on the right axis on computed positions.
   * @return distance from ticks to the axis in pixels
   */
  virtual double drawTicks(double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                           int nbTicks, double subticksPositions[], int nbSubtics) = 0;

  /**
   * Check if the ticks do not conceal each others.
   */
  virtual bool checkTicks(double ticksPositions[], char * ticksLabels[],
                          char * labelsExponents[], int nbTicks) = 0;
  /*----------------------------------------------------------------------*/
  ComputeTicksStrategy * m_pTicksComputer;

  GridDrawer * m_pGridDrawer;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _TICKS_DRAWER_HXX_ */
