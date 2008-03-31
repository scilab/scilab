/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TICKS_DRAWER_HXX_ 
#define _TICKS_DRAWER_HXX_

#include "../DrawableObjectBridge.h"
#include "ComputeTicksStrategy.hxx"
#include "DrawableSubwin.h"
#include "GridDrawer.hxx"
#include "PlaceTicksStrategy.hxx"

namespace sciGraphics
{

class TicksDrawer : public virtual DrawableObjectBridge
{
public:
  
  TicksDrawer(DrawableObject * drawer);

  virtual ~TicksDrawer(void);

  void setTicksComputer(ComputeTicksStrategy * ticksComputer);

  void setTicksPositioner(PlaceTicksStrategy * strategy);

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
   * Draw the grif if requested
   */
  void drawGrid(const double ticksPos[], const double subticksPos[],
                int nbTicks, int nbSubticks);

  /**
   * Set position of ticks and subTicks
   */
  virtual void setTicksPosition(const double ticksPosX[],
                                const double ticksPosY[],
                                const double ticksPosZ[],
                                int nbTicks) = 0;

  /**
   * Sepcify position of subticks
   */
  virtual void setSubticksPosition(const double subticksPosX[],
                                   const double subticksPosY[],
                                   const double subticksPosZ[],
                                   int nbSubticks) = 0;

  /**
   * Specify labels to draw in front of ticks
   */
  virtual void setTicksLabels(char * ticksLabels[],
                              char * labelsExponent[],
                              int nbLabels) = 0;

  /**
   * Specify axis segment position and ticks segments
   */
  virtual void setAxisPosition(const double axisSegmentStart[3],
                               const double axisSegmentEnd[3],
                               const double ticksDirection[3]) = 0;

  /**
   * Draw the ticks using precomputed positions
   * @return distance from ticks to the axis in pixels
   */
  virtual double concreteDrawTicks(void) = 0;

  /**
   * @return false if some of the ticks concealed and must be decimated
   *         true if ticks are correct and can be displayed
   */
  virtual bool checkTicks(void) = 0;

  /*----------------------------------------------------------------------*/
  ComputeTicksStrategy * m_pTicksComputer;

  PlaceTicksStrategy * m_pTicksPositioner;

  GridDrawer * m_pGridDrawer;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _TICKS_DRAWER_HXX_ */

