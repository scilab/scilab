/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy decomposing only segs object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsDecomposer.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
}

namespace sciGraphics
{
  /*---------------------------------------------------------------------------------*/
SegsDecomposer::SegsDecomposer(DrawableSegs * segs)
 : DecomposeSegsStrategy(segs) 
{

}
/*---------------------------------------------------------------------------------*/
SegsDecomposer::~SegsDecomposer(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsDecomposer::getSegsPos(double startXCoords[], double endXCoords[],
                                double startYCoords[], double endYCoords[],
                                double startZCoords[], double endZCoords[])
{
  int nbSegs = getNbSegment();
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);

  for (int i = 0; i < nbSegs; i++)
  {
    startXCoords[i] = ppSegs->vx[2 * i];
    endXCoords[i] = ppSegs->vx[2 * i + 1];
    startYCoords[i] = ppSegs->vy[2 * i];
    endYCoords[i] = ppSegs->vy[2 * i + 1];
  }

  if (ppSegs->vz != NULL)
  {
    for (int i = 0; i < nbSegs; i++)
    {
      startZCoords[i] = ppSegs->vz[2 * i];
      endZCoords[i] = ppSegs->vz[2 * i + 1];
    }
  }
  else
  {
    char logFlags[3];
    double defaultZvalue;
    sciGetLogFlags(sciGetParentSubwin(pSegs), logFlags);
    if (logFlags[2] == 'l')
    {
      defaultZvalue = 1.0;
    }
    else
    {
      defaultZvalue = 0.0;
    }

    for (int i = 0; i < nbSegs; i++)
    {
      startZCoords[i] = defaultZvalue;
      endZCoords[i] = defaultZvalue; 
    }
  }

  // apply log scale if needed
  m_pDrawed->pointScale(startXCoords, startYCoords, startZCoords, nbSegs);
  m_pDrawed->pointScale(endXCoords, endYCoords, endZCoords, nbSegs);


}
/*---------------------------------------------------------------------------------*/
int SegsDecomposer::getNbSegment(void)
{
  return pSEGS_FEATURE(m_pDrawed->getDrawedObject())->Nbr1 / 2;
}
/*---------------------------------------------------------------------------------*/
bool SegsDecomposer::isColored(void)
{
  // for now
  return true;
}
/*---------------------------------------------------------------------------------*/
void SegsDecomposer::getSegsColors(int colors[])
{
  int nbSegs = getNbSegment();
  sciSegs * ppSegs = pSEGS_FEATURE(m_pDrawed->getDrawedObject());
  intArrayCopy(colors, ppSegs->pstyle, nbSegs);
}
/*---------------------------------------------------------------------------------*/
}
