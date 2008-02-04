/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableGrayplot.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent routine for drawing     */
/*        a grayplot object                                               */
/*------------------------------------------------------------------------*/

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
  
  double * xGrid = new double[nbRow];
  double * yGrid = new double[nbCol];
  int * colors = new int[nbColors];

  // compute the grid and the colors to display
  m_pDecomposer->decomposeGrayplot(xGrid, yGrid, colors);

  // draw the grid on the screen
  getGrayplotImp()->drawGrayplot(xGrid, nbRow, yGrid, nbCol,
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
