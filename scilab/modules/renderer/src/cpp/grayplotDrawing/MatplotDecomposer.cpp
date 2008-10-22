/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy decomposing matplot objects
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "MatplotDecomposer.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
MatplotDecomposer::MatplotDecomposer(DrawableGrayplot * grayplot)
  : GrayplotDecompositionStrategy(grayplot)
{
  
}
/*---------------------------------------------------------------------------------*/
MatplotDecomposer::~MatplotDecomposer(void)
{

}
/*---------------------------------------------------------------------------------*/
int MatplotDecomposer::getNbRow(void)
{
  return pGRAYPLOT_FEATURE(m_pDrawed->getDrawedObject())->nx;
}
/*---------------------------------------------------------------------------------*/
int MatplotDecomposer::getNbCol(void)
{
  return pGRAYPLOT_FEATURE(m_pDrawed->getDrawedObject())->ny;
}
/*---------------------------------------------------------------------------------*/
int MatplotDecomposer::getNbColors(void)
{
  // we draw insie the grid
  return (getNbRow() - 1) * (getNbCol() - 1);
}
/*---------------------------------------------------------------------------------*/
double MatplotDecomposer::getZCoordinate(void)
{
  return 0.0;
}
/*---------------------------------------------------------------------------------*/
void MatplotDecomposer::decomposeGrayplot(double xGrid[], double yGrid[], int colors[])
{
  sciPointObj * pGray = m_pDrawed->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);
  int nbRow = getNbRow();
  int nbCol = getNbCol();

  if (ppGray->type == 1)
  {
    decomposeMatplot(xGrid, yGrid);
  }
  else
  {
    decomposeMatplot1(xGrid, yGrid);
  }

  // fill color
  for (int j = 0; j < nbRow - 1; j++)
  {
    for (int i = 0; i < nbCol - 1; i++)
    {
      // scilab data are stored column wise
      colors[i + (nbCol - 1) * j] = (int) ppGray->pvecz[j + (nbRow - 1) * i];
    }
  }

  // apply log scale if needed
  m_pDrawed->pointScale(xGrid, NULL, NULL, nbRow);
  m_pDrawed->pointScale(NULL, yGrid, NULL, nbCol);

}
/*---------------------------------------------------------------------------------*/
void MatplotDecomposer::decomposeMatplot(double xGrid[], double yGrid[])
{
  int nbRow = getNbRow();
  int nbCol = getNbCol();

  // the center of each facet is on (i,j) where i and j are integer
  // so facet (i,j) vertices are (i-0.5,j-0.5), (i+0.5,j-0.5),
  // (i+0.5,j+0.5), (i-0.5,j+0.5)

  // fill xgrid
  for (int i = 0; i < nbCol; i++)
  {
    // we start at i = 0, but matrix indices at 1
    xGrid[i] = i + 0.5;
  }

  // fill y grid
  // top of the matrix starts on Y max
  for (int j = 0; j < nbRow; j++)
  {
     // so the result is j - 0.5 + 1
    yGrid[j] = nbRow - j - 0.5;
  }
}
/*---------------------------------------------------------------------------------*/
void MatplotDecomposer::decomposeMatplot1(double xGrid[], double yGrid[])
{
  int nbRow = getNbRow();
  int nbCol = getNbCol();

  sciPointObj * pGray = m_pDrawed->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);

  // get data bounds
  double xMin = ppGray->pvecx[0];
  double xMax = ppGray->pvecx[2];
  double yMin = ppGray->pvecx[1];
  double yMax = ppGray->pvecx[3];

  // the matplot should fit in [xMin,xMax]x[yMin,yMax]

  // fill xgrid
  for (int i = 0; i < nbCol; i++)
  {
    xGrid[i] = xMin + i * (xMax - xMin) / (nbCol - 1);
  }

  // top of the matrix starts on Y max
  for (int j = 0; j < nbRow; j++)
  {
    yGrid[j] = yMax - j * (yMax - yMin) / (nbRow - 1);
  }
}
/*---------------------------------------------------------------------------------*/
}
