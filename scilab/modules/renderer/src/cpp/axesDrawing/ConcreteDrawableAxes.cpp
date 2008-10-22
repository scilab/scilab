/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the drivers independent routines to draw an axes object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableAxes.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableAxes::ConcreteDrawableAxes(sciPointObj * pAxes)
  : DrawableAxes(pAxes)
{
  m_pTicksDrawer = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableAxes::~ConcreteDrawableAxes(void)
{
  setTicksDrawer(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::setTicksDrawer(TicksDrawer * ticksDrawer)
{
  if (m_pTicksDrawer != NULL)
  {
    delete m_pTicksDrawer;
  }
  m_pTicksDrawer = ticksDrawer;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::drawAxes(void)
{
  m_pTicksDrawer->draw();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableAxes::showAxes(void)
{
  m_pTicksDrawer->show();
}
/*---------------------------------------------------------------------------------*/

}
