/*------------------------------------------------------------------------*/
/* file: AxesTicksComputer.cpp                                            */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Compute axes ticks                                              */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "AxesTicksComputer.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
#include "Format.h"
#include "DestroyObjects.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
AxesTicksComputer::AxesTicksComputer(DrawableAxes * axes)
: ComputeTicksStrategy(getSubwinDrawer(sciGetParentSubwin(axes->getDrawedObject())))
{
  m_pAxes = axes;
}
/*---------------------------------------------------------------------------------*/
AxesTicksComputer::~AxesTicksComputer(void)
{
  m_pAxes = NULL;
}
/*---------------------------------------------------------------------------------*/
void AxesTicksComputer::reinit(void)
{
  // nothing to do
}
/*---------------------------------------------------------------------------------*/
int AxesTicksComputer::getNbTicks(void)
{
  int nbTicks = 0;
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);
  double * positions = NULL;

  ComputeXIntervals(pAxes, ppAxes->tics, &positions, &nbTicks, 0);
  
  destroyGraphicPointer(positions);
  positions = NULL;

  return nbTicks;
}
/*---------------------------------------------------------------------------------*/
void AxesTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{
  int nbTicks = 0;
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);

  // get ticks
  double * tempPos = NULL;
  ComputeXIntervals(pAxes, ppAxes->tics, &tempPos, &nbTicks, 0);

  // copy back temp pos
  for (int i = 0; i < nbTicks; i++)
  {
    positions[i] = tempPos[i];
  }
  destroyGraphicPointer(tempPos);

  // get labels
  if (ppAxes->str == NULL)
  {
    // we need to rebuild it
    char format[5];
    ComputeC_format(pAxes, format) ;

    // 256 taken from old code
    char ** tempLabels = copyFormatedArray( positions, nbTicks, format, 256 ) ;

    for (int i = 0; i < nbTicks; i++)
    {
      labels[i] = new char[strlen(tempLabels[i]) + 1];
      strcpy(labels[i], tempLabels[i]);
    }

    // free strings with FREE
    destroyGraphicStringArray(tempLabels, nbTicks);


  }
  else
  {
    // copy str into labels
    for (int i = 0; i < nbTicks; i++)
    {
      labels[i] = new char[strlen(ppAxes->str[i]) + 1];
      strcpy(labels[i], ppAxes->str[i]);
    }
  }

}
/*---------------------------------------------------------------------------------*/
int AxesTicksComputer::getNbSubticks(double ticksPositions[], int nbTicks)
{
  sciPointObj * pAxes = m_pAxes->getDrawedObject();
  sciAxes * ppAxes = pAXES_FEATURE(pAxes);
  return Max(0, (ppAxes->subint - 1) * (nbTicks - 1));
}
/*---------------------------------------------------------------------------------*/
void AxesTicksComputer::getSubticksPosition(const double ticksPositions[], int nbTicks,
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
/*---------------------------------------------------------------------------------*/
void AxesTicksComputer::reduceTicksNumber(void)
{
  // nothing to do here
  // the ticks number can not change
}
/*---------------------------------------------------------------------------------*/

}
