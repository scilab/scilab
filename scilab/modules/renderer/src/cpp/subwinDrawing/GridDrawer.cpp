/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing grid on subwin axes 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GridDrawer.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
GridDrawer::GridDrawer(DrawableSubwin * subwin)
  : DrawableObjectBridge()
{
  m_bIsLogModeOn = false;
}
/*------------------------------------------------------------------------------------------*/
GridDrawer::~GridDrawer(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawer::draw(const double ticksPositions[], int nbTicks,
                      const double subticksPositions[], int nbSubticks)
{
  

  
  if(m_bIsLogModeOn)
  { 
    // in logarithmic mode ticks must also been drawn in front of subticks
    // so create a larger grid
    double * gridAbscissas = new double[nbTicks + nbSubticks];
    for (int i = 0; i < nbTicks; i++)
    {
      gridAbscissas[i] = ticksPositions[i];
    }
    for (int i = 0; i < nbSubticks; i++)
    {
      gridAbscissas[i + nbTicks] = subticksPositions[i];
    }
    drawGrid(subticksPositions, nbSubticks);
    delete[] gridAbscissas;
  }
  else
  {
    drawGrid(ticksPositions, nbTicks);
  }

}
/*------------------------------------------------------------------------------------------*/

}
