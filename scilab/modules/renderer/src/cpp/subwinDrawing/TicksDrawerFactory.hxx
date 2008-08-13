/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for TicksDrawer classes      
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TICKS_DRAWER_FACTORY_HXX_ 
#define _TICKS_DRAWER_FACTORY_HXX_

#include "DrawableSubwin.h"
#include "TicksDrawer.hxx"
#include "ComputeSubticksStrategy.hxx"
#include "ComputeTicksStrategy.hxx"

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

private:

  /**
   * Create a subticks computer.
   */
  ComputeSubticksStrategy * createRightSubTicksComputer(DrawableSubwin * subwin,
                                                        bool isAuto,
                                                        char logFlag,
                                                        int defaultNbSubticks);

};

}

#endif /* _TICKS_DRAWER_FACTORY_HXX_ */

