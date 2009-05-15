/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy decomposing grayplot objects
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GrayplotDecomposer.hxx"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
GrayplotDecomposer::GrayplotDecomposer(DrawableGrayplot * grayplot)
  : GrayplotDecompositionStrategy(grayplot)
{
  
}
/*---------------------------------------------------------------------------------*/
GrayplotDecomposer::~GrayplotDecomposer(void)
{
  
}
/*---------------------------------------------------------------------------------*/
int GrayplotDecomposer::getNbRow(void)
{
  return pGRAYPLOT_FEATURE(m_pDrawed->getDrawedObject())->ny;
}
/*---------------------------------------------------------------------------------*/
int GrayplotDecomposer::getNbCol(void)
{
  return pGRAYPLOT_FEATURE(m_pDrawed->getDrawedObject())->nx;
}
/*---------------------------------------------------------------------------------*/
int GrayplotDecomposer::getNbColors(void)
{
  // we draw insie the grid
  return (getNbRow() - 1) * (getNbCol() - 1);
}
/*---------------------------------------------------------------------------------*/
double GrayplotDecomposer::getZCoordinate(void)
{
  return 0.0;
}
/*---------------------------------------------------------------------------------*/
void GrayplotDecomposer::decomposeGrayplot(double xGrid[], double yGrid[], int colors[])
{
  sciPointObj * pGray = m_pDrawed->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);
  int nbRow = getNbRow();
  int nbCol = getNbCol();

  // fill xgrid
  for (int i = 0; i < nbCol; i++)
  {
    xGrid[i] = ppGray->pvecx[i];
  }

  // fill y grid
  for (int j = 0; j < nbRow; j++)
  {
    yGrid[j] = ppGray->pvecy[j];
  }

  // fill color
  if (ppGray->datamapping[0] == 's')
  {
    // s for scaled
    decomposeScaledColors(colors);
  }
  else
  {
    // d for direct
    decomposeDirectColors(colors);
  }

  // apply log scale if needed
  m_pDrawed->pointScale(xGrid, NULL, NULL, nbCol);
  m_pDrawed->pointScale(NULL, yGrid, NULL, nbRow);

}
/*---------------------------------------------------------------------------------*/
void GrayplotDecomposer::decomposeScaledColors(int colors[])
{
  int nbRow = getNbRow();
  int nbCol = getNbCol();

  sciPointObj * pGray = m_pDrawed->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);

  double lowColor;
  double highColor;
	bool colorInit = false;

  // first find lower and higher Z values
  for (int i = 0; i < nbRow * nbCol; i++)
  {
    double curColor = ppGray->pvecz[i];
		if (finite(curColor))
		{
			if (!colorInit)
			{
				// first non %nan color
				lowColor = curColor;
				highColor = curColor;
				colorInit = true;
			}
			else if (curColor > highColor)
			{
				highColor = curColor;
			}
			else if (curColor < lowColor)
			{
				lowColor = curColor;
			}
		}
		
  }


  // prevent from dividing by 0
  double zRange = highColor - lowColor;
  if (zRange < SMDOUBLE)
  {
    zRange = SMDOUBLE;
  }
  int colorMapSize = sciGetNumColors(sciGetParentFigure(pGray));

  // fill color
  for (int j = 0; j < nbRow - 1; j++)
  {
    for (int i = 0; i < nbCol - 1; i++)
    {
      // current value is the average on the facet
      double curZvalue = getFacetZvalue(ppGray->pvecz, nbCol, i, j);
      // colors is of size (nx - 1) x (ny -1) and pvecz of size nx x ny.
      // a 1 is added here and I don't know why
      // scilab data are stored column wise
      colors[i + (nbCol - 1) * j]
        = 1 + (int) floor( (colorMapSize - 1) * (curZvalue - lowColor) / zRange + 0.5);
    }
  }
}
/*---------------------------------------------------------------------------------*/
void GrayplotDecomposer::decomposeDirectColors(int colors[])
{
  int nbRow = getNbRow();
  int nbCol = getNbCol();
  sciPointObj * pGray = m_pDrawed->getDrawedObject();
  sciGrayplot * ppGray = pGRAYPLOT_FEATURE(pGray);


  // fill color
  for (int i = 0; i < nbRow - 1; i++)
  {
    for (int j = 0; j < nbCol - 1; j++)
    {
      // colors is of size (nx - 1) x (ny -1) and pvecz of size nx x ny.
      // scilab data are stored column wise
      colors[j + (nbCol - 1) * i] = (int) ppGray->pvecz[i + nbCol * j];
    }
  }

}
/*---------------------------------------------------------------------------------*/
double GrayplotDecomposer::getFacetZvalue(const double zValues[], int nbCol, int i, int j)
{
  // mean on the facet
   return 0.25 * (  zValues[i + nbCol * j] + zValues[i + 1 + nbCol * j]
                  + zValues[i + 1 + nbCol * (j + 1)] + zValues[i + nbCol * (j + 1)]);
}
/*---------------------------------------------------------------------------------*/
}

