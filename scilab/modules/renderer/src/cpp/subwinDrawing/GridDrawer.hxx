/*------------------------------------------------------------------------*/
/* file: GridDrawer.hxx                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid on subwin axes                */
/*------------------------------------------------------------------------*/

#ifndef _GRID_DRAWER_HXX_ 
#define _GRID_DRAWER_HXX_

#include "../DrawableObjectBridge.h"
#include "DrawableSubwin.h"
#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class GridDrawer : public virtual DrawableObjectBridge
{
public:

  GridDrawer(DrawableSubwin * subwin);

  virtual ~GridDrawer(void);

  /**
   * Draw the grid for one axis
   */
  void draw(const double ticksPositions[], int nbTicks,
            const double subticksPositions[], int nbSubticks);

  /**
   * Set whether the logarithmic mode is on or of.
   */
  void setLogMode(bool isOn) {m_bIsLogModeOn = isOn;}

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Draw the grid
   */
  virtual void drawGrid(const double gridPositions[], int nbPositions) = 0;
  /*----------------------------------------------------------------------*/
  /** To know if we need to draw subticks or not */
  bool m_bIsLogModeOn;
  /*----------------------------------------------------------------------*/

};

}

#endif /* _GRID_DRAWER_HXX_ */
