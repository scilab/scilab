/*------------------------------------------------------------------------*/
/* file: UserDefinedTicksComputer.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute ticks from ticks given by the user                      */
/*------------------------------------------------------------------------*/

#include "UserDefinedTicksComputer.hxx"
#include "BasicAlgos.hxx"


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UserDefinedTicksComputer::UserDefinedTicksComputer(DrawableSubwin * subwin)
  : ComputeTicksStrategy(subwin)
{
  m_aUserTicksPositions = NULL;
  m_iNbUserTicks = 0;
}
/*------------------------------------------------------------------------------------------*/
UserDefinedTicksComputer::~UserDefinedTicksComputer(void)
{
  m_aUserTicksPositions = NULL;
  m_iNbUserTicks = 0;
}
/*------------------------------------------------------------------------------------------*/
int UserDefinedTicksComputer::getNbTicks(void)
{
  return m_iNbUserTicks;
}
/*------------------------------------------------------------------------------------------*/
void UserDefinedTicksComputer::getTicksPosition(double positions[], char * labels[])
{
  for (int i = 0; i < m_iNbUserTicks; i++)
  {
    positions[i] = m_aUserTicksPositions[i];
  }
  BasicAlgos::stringArrayCopy(labels, m_aUserTicksLabels, m_iNbUserTicks);
}
/*------------------------------------------------------------------------------------------*/
int UserDefinedTicksComputer::getNbSubticks(void)
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
