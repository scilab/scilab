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

#include "DrawableObjectBridge.h"
#include "ComputeTicksStrategy.hxx"
#include "ComputeSubticksStrategy.hxx"
#include "DrawableSubwin.h"
#include "GridDrawer.hxx"
#include "AxisPositioner.hxx"
#include "TicksDrawerBridge.hxx"

namespace sciGraphics
{

class TicksDrawer
{
public:
  
  TicksDrawer(void);

  virtual ~TicksDrawer(void);

  void setTicksComputer(ComputeTicksStrategy * ticksComputer);

  void setSubticksComputer(ComputeSubticksStrategy * subticksComputer);

  void setGridDrawer(GridDrawer * gridDrawer);

  void setAxisPositioner(AxisPositioner * positioner);

  void setTicksDrawer(TicksDrawerBridge * drawer);

  /**
   * To be called before any draw
   */
  void initializeDrawing(void);

  /**
   * To be called after any draw
   */
  void endDrawing(void);

  /**
   * Main function which draw ticks.
   */
  double draw(void);

  /**
   * Redefined show since we can not use display lists.
   */
  double show(void);

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn
   */
  int getInitNbTicks(void);

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  void getInitTicksPos(double ticksPositions[], char ** ticksLabels);

  /**
   * Compute bounds of the axis and direction of ticks
   */
  void getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]);

protected:

  /*----------------------------------------------------------------------*/
  /**
   * Compute ticks positions and draw ticks from the computed position.
   */
  double drawTicks(void);

   /**
   * Redefined show since we can not use display lists.
   */
  double showTicks(void);
  /*----------------------------------------------------------------------*/
  ComputeTicksStrategy * m_pTicksComputer;

  ComputeSubticksStrategy * m_pSubticksComputer;

  AxisPositioner * m_pPositioner;

  GridDrawer * m_pGridDrawer;

  TicksDrawerBridge * m_pTicksDrawer;
  /*----------------------------------------------------------------------*/
};

}

#endif /* _TICKS_DRAWER_HXX_ */

