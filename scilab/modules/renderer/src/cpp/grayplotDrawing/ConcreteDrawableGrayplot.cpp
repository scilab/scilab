/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver independent routine for drawing a grayplot object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableGrayplot.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
ConcreteDrawableGrayplot::ConcreteDrawableGrayplot(sciPointObj * pObj)
  : DrawableGrayplot(pObj)
{
  m_pDecomposer = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableGrayplot::~ConcreteDrawableGrayplot(void)
{
  setDeccompositionStrategy(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableGrayplot::setDeccompositionStrategy(GrayplotDecompositionStrategy * strategy)
{
  if (m_pDecomposer != NULL)
  {
    delete m_pDecomposer;
  }
  m_pDecomposer = strategy;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableGrayplot::drawGrayplot(void)
{
  int nbRow = m_pDecomposer->getNbRow();
  int nbCol = m_pDecomposer->getNbCol();
  int nbColors = m_pDecomposer->getNbColors();
  
  // beware here the column are for X
  // row on Y
  double * xGrid = new double[nbCol];
  double * yGrid = new double[nbRow];
  int * colors = new int[nbColors];

  // compute the grid and the colors to display
  m_pDecomposer->decomposeGrayplot(xGrid, yGrid, colors);

  // draw the grid on the screen
  getGrayplotImp()->drawGrayplot(xGrid, nbCol, yGrid, nbRow,
                                 m_pDecomposer->getZCoordinate(),
                                 colors, nbColors);

  delete[] xGrid;
  delete[] yGrid;
  delete[] colors;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableGrayplot::showGrayplot(void)
{
  getDrawableImp()->show();
}
/*---------------------------------------------------------------------------------*/
}
