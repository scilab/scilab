/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Compute ticks from ticks given by the user in logarithmic mode 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>

#include "UserDefLogTicksComputer.hxx"
#include "BasicAlgos.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UserDefLogTicksComputer::UserDefLogTicksComputer(DrawableSubwin * subwin)
 : UserDefinedTicksComputer(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
UserDefLogTicksComputer::~UserDefLogTicksComputer(void)
{

}
/*------------------------------------------------------------------------------------------*/
void UserDefLogTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{
  for (int i = 0; i < m_iNbUserTicks; i++)
  {
    positions[i] = m_aUserTicksPositions[i];
  }
  BasicAlgos::stringArrayCopy(labels, m_aUserTicksLabels, m_iNbUserTicks);
}
/*------------------------------------------------------------------------------------------*/
}
