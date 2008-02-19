/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Compute ticks from ticks given by the user 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "UserDefinedTicksComputer.hxx"
#include "BasicAlgos.hxx"


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UserDefinedTicksComputer::UserDefinedTicksComputer(DrawableSubwin * subwin)
  : ComputeTicksStrategy(subwin)
{
  m_aUserTicksPositions = NULL;
  m_aUserTicksLabels = NULL;
  m_iNbUserTicks = 0;
  m_iNbSubticks = 0;
}
/*------------------------------------------------------------------------------------------*/
UserDefinedTicksComputer::~UserDefinedTicksComputer(void)
{
  m_aUserTicksPositions = NULL;
  m_aUserTicksLabels = NULL;
  m_iNbUserTicks = 0;
  m_iNbSubticks = 0;
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedTicksComputer::reinit(void)
{

}
/*------------------------------------------------------------------------------------------*/
int UserDefinedTicksComputer::getNbTicks(void)
{
  return m_iNbUserTicks;
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{
  for (int i = 0; i < m_iNbUserTicks; i++)
  {
    positions[i] = m_aUserTicksPositions[i];
  }
  BasicAlgos::stringArrayCopy(labels, m_aUserTicksLabels, m_iNbUserTicks);
}
/*------------------------------------------------------------------------------------------*/
int UserDefinedTicksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  return Max(0, m_iNbSubticks * (m_iNbUserTicks - 1));
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedTicksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
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
void UserDefinedTicksComputer::reduceTicksNumber(void)
{
  // don't do anything since this is user defined ticks
  // Actually, this function should not be called.
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedTicksComputer::setUserTicks(double * userTicks, char ** userLabels,
                                            int nbUserTicks, int nbSubticks)
{
  m_aUserTicksPositions = userTicks;
  m_aUserTicksLabels = userLabels;
  m_iNbUserTicks = nbUserTicks;
  m_iNbSubticks = nbSubticks;
}
/*------------------------------------------------------------------------------------------*/
}
