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
GridDrawer::GridDrawer(void)
  : DrawableObjectBridge()
{
  m_bIsLogModeOn = false;
}
/*------------------------------------------------------------------------------------------*/
GridDrawer::~GridDrawer(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawer::draw(const double firstAxisStart[3], const double firstAxisEnd[3],
                      const double secondAxisStart[3], const double secondAxisEnd[3],
                      const double thirdAxisStart[3], const double thirdAxisEnd[3],
                      const double relativeTicksPositions[], int nbTicks,
                      const double relativeSubticksPositions[], int nbSubticks)
{
  

  
  if(m_bIsLogModeOn)
  { 
    // in logarithmic mode ticks must also been drawn in front of subticks
    // so create a larger grid
    double * gridAbscissas = new double[nbTicks + nbSubticks];
    for (int i = 0; i < nbTicks; i++)
    {
      gridAbscissas[i] = relativeTicksPositions[i];
    }
    for (int i = 0; i < nbSubticks; i++)
    {
      gridAbscissas[i + nbTicks] = relativeSubticksPositions[i];
    }
    drawGrid(firstAxisStart, firstAxisEnd,
             secondAxisStart, secondAxisEnd,
             thirdAxisStart, thirdAxisEnd,
             gridAbscissas, nbTicks + nbSubticks);
    delete[] gridAbscissas;
  }
  else
  {
    drawGrid(firstAxisStart, firstAxisEnd,
             secondAxisStart, secondAxisEnd,
             thirdAxisStart, thirdAxisEnd,
             relativeTicksPositions, nbTicks);
  }

}
/*------------------------------------------------------------------------------------------*/

}
