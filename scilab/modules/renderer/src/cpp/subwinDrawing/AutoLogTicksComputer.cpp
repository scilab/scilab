/*------------------------------------------------------------------------*/
/* file: AutoLogTicksComputer.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute automatic ticks with logarithmic scaling                */
/*------------------------------------------------------------------------*/
#include <math.h>

#include "AutoLogTicksComputer.hxx"

extern "C"
{
#include "Format.h"
#include "DrawObjects.h"
}

namespace sciGraphics
{

/** Base to display for logarithmix axis */
static const char LOG_BASE[] = "10";

/*------------------------------------------------------------------------------------------*/
AutoLogTicksComputer::AutoLogTicksComputer(DrawableSubwin * subwin)
  : AutomaticTicksComputer(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
AutoLogTicksComputer::~AutoLogTicksComputer(void)
{

}
/*------------------------------------------------------------------------------------------*/
int AutoLogTicksComputer::getNbTicks(void)
{

  if (m_iNbTicks < 0)
  {
    double ticks[20];
    GradLog(m_dMinBounds, m_dMaxBounds, ticks, &m_iNbTicks, FALSE);
  }
  return m_iNbTicks;
}
/*------------------------------------------------------------------------------------------*/
void AutoLogTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{
  if (m_iNbTicks < 0)
  {
    getNbTicks();
  }

  // Number of ticks has already been computed.
  GradLog(m_dMinBounds, m_dMaxBounds, positions, &m_iNbTicks, TRUE);

  // ticks labels are 10^i
  // i is computed by grad log
  char labelsExponentFormat[5];
  int lastIndex = Max( m_iNbTicks - 1, 0 ) ;

  ChoixFormatE( labelsExponentFormat,
                positions[0],
                positions[lastIndex],
                (positions[lastIndex] - positions[0]) / lastIndex ); /* Adding F.Leray 06.05.04 */
  
  // copy exponents
  char buffer[64];
  for (int i = 0; i < m_iNbTicks; i++)
  {
    // convert current position into a string
    sprintf(buffer, labelsExponentFormat, positions[i]);

    // add the string to labels
    if (labelsExponents[i] != NULL) {delete labelsExponents[i];}

    labelsExponents[i] = new char[strlen(buffer) + 1];
    strcpy(labelsExponents[i], buffer);

    // copy "10" in each labem
    if  (labels[i] != NULL) {delete labels[i];}

    labels[i] = new char[strlen(LOG_BASE) + 1];
    strcpy(labels[i], LOG_BASE);
  }




}
/*------------------------------------------------------------------------------------------*/
int AutoLogTicksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  return Max(0, ComputeNbSubTics(m_pDrawer->getDrawedObject(), nbTicks, 'l', ticksPositions, 0) * (m_iNbTicks - 1));
}
/*------------------------------------------------------------------------------------------*/
void AutoLogTicksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
                                               double subTickspositions[])
{
  // compute number of subtics
  int nbSubtics = ComputeNbSubTics(m_pDrawer->getDrawedObject(), nbTicks, 'l', ticksPositions, 0);

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
      // positions are given in logarithmic mode
      // so we need to switch ticks positions in normal mode
      // and then come back to log mode
      // the expression is then s = log(exp(t[i]) + (exp(t[i+1]) - exp(t[i])) * (j+1) / (nb+1)
      // and simplified
      subTickspositions[j + nbSubtics * i]
        = prevTick + log10(1.0 + (exp10(nextTick - prevTick) - 1.0) * (j + 1.0) / (nbSubtics + 1.0));
    }
  }
}
/*------------------------------------------------------------------------------------------*/
void AutoLogTicksComputer::setAxisBounds(double min, double max)
{
  m_dMinBounds = log10(min);
  m_dMaxBounds = log10(max);
}
/*------------------------------------------------------------------------------------------*/
}
