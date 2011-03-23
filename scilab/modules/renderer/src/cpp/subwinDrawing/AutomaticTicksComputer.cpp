/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Compute automatic ticks
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "AutomaticTicksComputer.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "Format.h"
#include "DrawObjects.h"
#include "math_graphics.h"
}

namespace sciGraphics
{

static const int BUFFER_LENGTH = 64;

/*------------------------------------------------------------------------------------------*/
AutomaticTicksComputer::AutomaticTicksComputer(DrawableSubwin * subwin)
  : ComputeTicksStrategy(subwin)
{
  m_dMinBounds = 0.0;
  m_dMinBounds = 0.0;
  m_iNbTicks = -1; /* ie unitialized */
}
/*------------------------------------------------------------------------------------------*/
AutomaticTicksComputer::~AutomaticTicksComputer(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void AutomaticTicksComputer::reinit(void)
{
  m_iNbTicks = -1;
}
/*------------------------------------------------------------------------------------------*/
int AutomaticTicksComputer::getNbTicks(void)
{

  if (m_iNbTicks < 0)
  {
		// ticks not already decimated
		int nbTicks = 0;
    double ticks[20];
    TheTicks(&m_dMinBounds, &m_dMaxBounds, ticks, &nbTicks, FALSE);
		return nbTicks;
  }
	else
	{
		// ticks decimated, use the specified value
		return m_iNbTicks;
	}
}
/*------------------------------------------------------------------------------------------*/
void AutomaticTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{

	if (m_iNbTicks < 0)
	{
		// TheTicks gives different results if 
  	// number of ticks computation is on or off, so we need to compute number of
	  // ticks again.
		TheTicks(&m_dMinBounds, &m_dMaxBounds, positions, &m_iNbTicks, FALSE);
	}
	else
	{
		TheTicks(&m_dMinBounds, &m_dMaxBounds, positions, &m_iNbTicks, TRUE);
	}

  /* Build the tick labels if the labels arguement is not null.  (labelsExponents is unused.) */
  if( labels != NULL )
  {
    // find ticks format
    char labelsFormat[5];
    int lastIndex = Max( m_iNbTicks - 1, 0 ) ;

    ChoixFormatE( labelsFormat,
                positions[0],
                positions[lastIndex],
                (positions[lastIndex] - positions[0]) / lastIndex ); /* Adding F.Leray 06.05.04 */

    char buffer[BUFFER_LENGTH];
    for (int i = 0; i < m_iNbTicks; i++)
    {
      // convert current position into a string
      sprintf(buffer, labelsFormat, positions[i]);

      // add the string to labels
      if (labels[i] != NULL) {delete[] labels[i];}

      labels[i] = new char[strlen(buffer) + 1];
      strcpy(labels[i], buffer);
    }
  }

}
/*------------------------------------------------------------------------------------------*/
void AutomaticTicksComputer::reduceTicksNumber(void)
{
  m_iNbTicks = reduceTicksNumber(m_iNbTicks);
}
/*------------------------------------------------------------------------------------------*/
int AutomaticTicksComputer::reduceTicksNumber(int numberOfTicks)
{
  return ((numberOfTicks +1) / 2);
}
/*------------------------------------------------------------------------------------------*/
int AutomaticTicksComputer::computeMaxNumberOfDecimationIterations(void)
{
  int nbIterations = 0;
  int numberOfTicks = m_iNbTicks;

  // No iterations are performed since decimation requires
  // more than one tick
  if (numberOfTicks <= 1)
  {
    return 0;
  }

  while(numberOfTicks > 1)
  {
    numberOfTicks = reduceTicksNumber(numberOfTicks);
    nbIterations++;
  }

  return nbIterations;
}
/*------------------------------------------------------------------------------------------*/
void AutomaticTicksComputer::setAxisBounds(double min, double max)
{
  m_dMinBounds = min;
  m_dMaxBounds = max;
}
/*------------------------------------------------------------------------------------------*/
}
