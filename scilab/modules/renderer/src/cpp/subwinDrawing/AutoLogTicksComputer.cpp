/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Compute automatic ticks with logarithmic scaling
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AutoLogTicksComputer.hxx"

extern "C"
{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Format.h"
#include "DrawObjects.h"
#include "math_graphics.h"
}

namespace sciGraphics
{

/** Base to display for logarithmix axis */
static const char LOG_BASE[] = "10";

static const int BUFFER_LENGTH = 64;

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
		// ticks not already decimated
		int nbTicks = 0;
    double ticks[20];
    GradLog(m_dMinBounds, m_dMaxBounds, ticks, &nbTicks, FALSE);
		return nbTicks;
  }
	else
	{
		// ticks decimated, use the specified value
		return m_iNbTicks;
	}
}
/*------------------------------------------------------------------------------------------*/
void AutoLogTicksComputer::getTicksPosition(double positions[], char * labels[], char * labelsExponents[])
{

  if (m_iNbTicks < 0)
  {
    GradLog(m_dMinBounds, m_dMaxBounds, positions, &m_iNbTicks, FALSE);
  }
  else
  {
    GradLog(m_dMinBounds, m_dMaxBounds, positions, &m_iNbTicks, TRUE);
  }

  /* Build the tick labels if neither labels nor labelsExponents is null. */
  if( labels != NULL &&  labelsExponents != NULL )
  {
    // ticks labels are 10^i
    // i is computed by grad log
    char labelsExponentFormat[5];
    int lastIndex = Max( m_iNbTicks - 1, 0 ) ;

    ChoixFormatE( labelsExponentFormat,
                positions[0],
                positions[lastIndex],
                (positions[lastIndex] - positions[0]) / lastIndex ); /* Adding F.Leray 06.05.04 */
  
    // copy exponents
    char buffer[BUFFER_LENGTH];
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

}
/*------------------------------------------------------------------------------------------*/

}
