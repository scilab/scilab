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
#include "UserDefinedSubticksComputer.hxx"

extern "C"
{
#include "Format.h"
#include "DrawObjects.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
UserDefinedSubticksComputer::UserDefinedSubticksComputer(DrawableSubwin * subwin)
  : ComputeSubticksStrategy(subwin)
{
  
}
/*------------------------------------------------------------------------------------------*/
UserDefinedSubticksComputer::~UserDefinedSubticksComputer(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
int UserDefinedSubticksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  return Max(0, m_iNbSubticks * (nbTicks - 1));
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedSubticksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
                                                    double subTickspositions[])
{
  /*    |              |              |    */
  /* ___|____|____|____|____|____|____|___ */
  /*   t0             t1             t2   */

  // draw only between two ticks, so skip last one
  for (int i = 0; i < nbTicks - 1; i++)
  {
    // decompose interval in nbsubtics parts
    double prevTick = ticksPositions[i];
    double nextTick = ticksPositions[i + 1];
    for (int j = 0; j < m_iNbSubticks; j++)
    {
      subTickspositions[j + m_iNbSubticks * i]
        =  prevTick + (nextTick - prevTick) * (j + 1.0) / (m_iNbSubticks + 1.0); 
    }
  }
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedSubticksComputer::setUserSubticks(int nbSubticks)
{
  m_iNbSubticks = nbSubticks;
}
/*------------------------------------------------------------------------------------------*/
}
