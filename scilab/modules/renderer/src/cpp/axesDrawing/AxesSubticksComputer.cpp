/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Compute automatic ticks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "AxesSubticksComputer.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
AxesSubticksComputer::AxesSubticksComputer(DrawableAxes * axes)
  : ComputeSubticksStrategy(getSubwinDrawer(sciGetParentSubwin(axes->getDrawedObject())))
{
  m_pAxes = axes;
}
/*------------------------------------------------------------------------------------------*/
AxesSubticksComputer::~AxesSubticksComputer(void)
{
  m_pAxes = NULL;
}
/*------------------------------------------------------------------------------------------*/
int AxesSubticksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);
  return Max(0, (ppAxes->subint - 1) * (nbTicks - 1));
}
/*------------------------------------------------------------------------------------------*/
void AxesSubticksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
                                                    double subTickspositions[])
{
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);
  // compute number of subtics per ticks interval
  int nbSubtics = ppAxes->subint - 1;

  /*    |              |              |    */
  /* ___|____|____|____|____|____|____|___ */
  /*   t0             t1             t2   */

  // draw only between two ticks, so skip last one
  for (int i = 0; i < nbTicks - 1; i++)
  {
    // decompose interval in nbsubtics parts
    double prevTick = ticksPositions[i];
    double nextTick = ticksPositions[i + 1];
    for (int j = 0; j < nbSubtics; j++)
    {
      subTickspositions[j + nbSubtics * i]
        =  prevTick + (nextTick - prevTick) * (j + 1.0) / (nbSubtics + 1.0); 
    }
  }
}
/*------------------------------------------------------------------------------------------*/
}
