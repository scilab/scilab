/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing ticks  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TICKS_DRAWER_BRIDGE_HXX_ 
#define _TICKS_DRAWER_BRIDGE_HXX_

#include "../DrawableObjectBridge.h"

namespace sciGraphics
{
 
class TicksDrawerBridge : public virtual DrawableObjectBridge
{
public:
  
  TicksDrawerBridge(void) {}

  virtual ~TicksDrawerBridge(void) {}

  /**
   * Redefined show since we can not use display lists.
   */
  virtual double showTicks(void) = 0;

  /**
   * Draw the ticks on the right axis on computed positions.
   * @return distance from ticks to the axis in pixels
   */
  virtual double drawTicks(double relativeTicksPositions[],
                           char * ticksLabels[],
                           char * labelsExponents[],
                           int nbTicks,
                           double relativeSubticksPositions[],
                           int nbSubtics,
                           double axisSegmentStart[3],
                           double axisSegmentEnd[3],
                           double ticksDirection[3]) = 0;


  /**
   * To know if we need to check that ticks are concealing each others or
   * not.
   */
  virtual void setNeedTicksDecimation(bool needDecimation) = 0;

protected:

  /*----------------------------------------------------------------------*/

};

}

#endif /* _TICKS_DRAWER_BRIDGE_HXX_ */

