/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Compute axes ticks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
void AxesTicksComputer::reduceTicksNumber(void)
{
  // nothing to do here
  // the ticks number can not change
}
/*---------------------------------------------------------------------------------*/

}
