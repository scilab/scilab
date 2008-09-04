/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy decomposing only champ object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ChampDecomposer.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
#include "math_graphics.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ChampDecomposer::ChampDecomposer(DrawableSegs * segs)
  : DecomposeSegsStrategy(segs) 
{

}
/*---------------------------------------------------------------------------------*/
ChampDecomposer::~ChampDecomposer(void)
{

}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getSegsPos(double startXCoords[], double endXCoords[],
                                 double startYCoords[], double endYCoords[],
                                 double startZCoords[], double endZCoords[])
{
  int nbSegs = getNbSegment();
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);

  // get data for X and Y values
  if (ppSegs->typeofchamp == 0)
  {
    getChampPos(startXCoords, endXCoords, startYCoords, endYCoords);
  }
  else
  {
    getChamp1Pos(startXCoords, endXCoords, startYCoords, endYCoords);
  }

  // champ is not yet usable in 3d, so use a default value for Z
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

  // apply log scale if needed
  m_pDrawed->pointScale(startXCoords, startYCoords, startZCoords, nbSegs);
  m_pDrawed->pointScale(endXCoords, endYCoords, endZCoords, nbSegs);


}
/*---------------------------------------------------------------------------------*/
int ChampDecomposer::getNbSegment(void)
{
  sciSegs * ppSegs = pSEGS_FEATURE(m_pDrawed->getDrawedObject());
  return ppSegs->Nbr1 * ppSegs->Nbr2;
}
/*---------------------------------------------------------------------------------*/
bool ChampDecomposer::isColored(void)
{
  // for now
  return true;
}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getSegsColors(int colors[])
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);

  // get data for X and Y values
  if (ppSegs->typeofchamp == 0)
  {
    getChampColors(colors);
  }
  else
  {
    getChamp1Colors(colors);
  }
}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getDefaultChampPos(double startXCoords[], double endXCoords[],
                                         double startYCoords[], double endYCoords[])
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);
  int curMatrixIndex = 0;

  for (int i = 0; i < ppSegs->Nbr1; i++)
  {
    for (int j = 0; j < ppSegs->Nbr2; j++)
    {
      curMatrixIndex = i + ppSegs->Nbr1 * j;
      startXCoords[curMatrixIndex] = ppSegs->vx[i];
      endXCoords[curMatrixIndex] = ppSegs->vx[i] + ppSegs->vfx[curMatrixIndex];
      startYCoords[curMatrixIndex] = ppSegs->vy[j];
      endYCoords[curMatrixIndex] = ppSegs->vy[j] + ppSegs->vfy[curMatrixIndex];
    }
  }
}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getChampPos(double startXCoords[], double endXCoords[],
                                  double startYCoords[], double endYCoords[])
{
  getDefaultChampPos(startXCoords, endXCoords, startYCoords, endYCoords);

  // find the length of the longest vector
  double curMaxLength = getMaxLength();
  double maxLength = computeMaxUsableLength();

  int nbSegs = getNbSegment();
  
  // modify the length proportionally so
  // that the longest vector get the max usable length
  for (int i = 0; i < nbSegs; i++)
  {
    double curVect[2] = {endXCoords[i] - startXCoords[i], endYCoords[i] - startYCoords[i]};
    endXCoords[i] = startXCoords[i] + curVect[0] * maxLength / curMaxLength;
    endYCoords[i] = startYCoords[i] + curVect[1] * maxLength / curMaxLength;
  }
}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getChamp1Pos(double startXCoords[], double endXCoords[],
                                   double startYCoords[], double endYCoords[])
{
  // same as getChampPos, but we modify the segs length
  // so that each segment has the same length as the longest vector
  getDefaultChampPos(startXCoords, endXCoords, startYCoords, endYCoords);

  // find the length of the longest vector
  double maxLength = computeMaxUsableLength();

  int nbSegs = getNbSegment();
  for (int i = 0; i < nbSegs; i++)
  {
    double curVect[2] = {endXCoords[i] - startXCoords[i], endYCoords[i] - startYCoords[i]};
    double curNorm = NORM_2D(curVect);
    endXCoords[i] = startXCoords[i] + curVect[0] * maxLength / curNorm;
    endYCoords[i] = startYCoords[i] + curVect[1] * maxLength / curNorm;
  }

}
/*---------------------------------------------------------------------------------*/
double ChampDecomposer::getMaxLength(void)
{
  int nbSegs = getNbSegment();
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);
  double maxLength = 0.0;
  for (int i = 0; i < nbSegs; i++)
  {
    double curLength = ppSegs->vfx[i] * ppSegs->vfx[i] + ppSegs->vfy[i] * ppSegs->vfy[i];
    if (curLength > maxLength)
    {
      maxLength = curLength;
    }
  }
  return sqrt(maxLength);
}
/*---------------------------------------------------------------------------------*/
double ChampDecomposer::computeMaxUsableLength(void)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);
  
  // find the minimum distance between two consecutive abscissas.
  double minLengthX;
  if (ppSegs->Nbr1 < 2)
  {
    minLengthX = 1.0;
  }
  else
  {
    minLengthX = Abs(ppSegs->vx[1] - ppSegs->vx[0]);
    for (int i = 1; i < ppSegs->Nbr1 - 1; i++)
    {
      double curLength = Abs(ppSegs->vx[i + 1] - ppSegs->vx[i]);
      if (curLength < minLengthX)
      {
        minLengthX = curLength;
      }
    }
  }
  
  
  // find the minimum distance between two consecutive ordinates.
  double minLengthY;

  if (ppSegs->Nbr2 < 2)
  {
    minLengthY = 1.0;
  }
  else
  {
    minLengthY = Abs(ppSegs->vy[1] - ppSegs->vy[0]);
    for (int i = 1; i < ppSegs->Nbr2 - 1; i++)
    {
      double curLength = Abs(ppSegs->vy[i + 1] - ppSegs->vy[i]);
      if (curLength < minLengthY)
      {
        minLengthY = curLength;
      }
    }
  }

  // get the minimum between the two
  return Min(minLengthX, minLengthY);

}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getChampColors(int colors[])
{
  int nbSegs = getNbSegment();
  for (int i = 0; i < nbSegs; i++)
  {
    colors[i] = 0;
  }
}
/*---------------------------------------------------------------------------------*/
void ChampDecomposer::getChamp1Colors(int colors[])
{
  int nbSegs = getNbSegment();
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();
  sciSegs * ppSegs = pSEGS_FEATURE(pSegs);
  int colorMapSize = sciGetNumColors(sciGetParentFigure(pSegs));
  double maxLength = getMaxLength();

  // prevent from dividing by 0.
  if (maxLength < SMDOUBLE)
  {
    maxLength = SMDOUBLE;
  }

  for (int i = 0; i < nbSegs; i++)
  {
    double curVectorNorm = sqrt(ppSegs->vfx[i] * ppSegs->vfx[i] + ppSegs->vfy[i] * ppSegs->vfy[i]);
    colors[i] = (int) floor(((colorMapSize - 1) * curVectorNorm / maxLength ) + 0.5);
  }

}
/*---------------------------------------------------------------------------------*/
}
