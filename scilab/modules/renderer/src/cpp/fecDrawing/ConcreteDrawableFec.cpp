/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver independent routines for drawing a fec object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableFec.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
ConcreteDrawableFec::ConcreteDrawableFec(sciPointObj * pFec)
  : DrawableFec(pFec)
{
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableFec::~ConcreteDrawableFec(void)
{
  removeDrawingStrategies();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::addDrawingStrategy(DrawFecStrategy * strategy)
{
  m_oDrawingStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::removeDrawingStrategies(void)
{
  list<DrawFecStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::drawFec(void)
{

  int nbNodes = getNbNodes();
  double * xCoords = new double[nbNodes];
  double * yCoords = new double[nbNodes];

  int nbTriangles = getNbTriangles();
  int * firstPoints  = new int[nbTriangles];
  int * secondPoints = new int[nbTriangles];
  int * thirdPoints  = new int[nbTriangles];

  decomposeFec(xCoords, yCoords, firstPoints, secondPoints, thirdPoints);

  drawFec(xCoords, yCoords, pFEC_FEATURE(m_pDrawed)->pfun, nbNodes,
          firstPoints, secondPoints, thirdPoints, nbTriangles);

  delete[] xCoords;
  delete[] yCoords;
  delete[] firstPoints;
  delete[] secondPoints;
  delete[] thirdPoints;

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::drawFec(const double xCoords[], const double yCoords[],
                                  const double values[], int nbNodes,
                                  const int firstPoints[], const int secondPoints[],
                                  const int thirdPoints[], int nbTriangles)
{
  list<DrawFecStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawFec(xCoords, yCoords, values, nbNodes,
                   firstPoints, secondPoints, thirdPoints, nbTriangles);
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::showFec(void)
{
  list<DrawFecStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->showFec();
  }
}
/*---------------------------------------------------------------------------------*/
int ConcreteDrawableFec::getNbNodes(void)
{
  return pFEC_FEATURE(m_pDrawed)->Nnode;
}
/*---------------------------------------------------------------------------------*/
int ConcreteDrawableFec::getNbTriangles(void)
{
  return pFEC_FEATURE(m_pDrawed)->Ntr;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableFec::decomposeFec(double xCoords[], double yCoords[],
                                       int firstPoints[], int secondPoints[], int thirdPoints[])
{
  int nbNodes = getNbNodes();
  sciFec * ppFec = pFEC_FEATURE(m_pDrawed);

  // get positions of nodes
  for (int i = 0; i < nbNodes; i++)
  {
    xCoords[i] = ppFec->pvecx[i];
    yCoords[i] = ppFec->pvecy[i];
  }

  // use log scale if needed
  pointScale(xCoords, yCoords, NULL, nbNodes);

  // get triangles positions
  int nbTriangles = getNbTriangles();
  for (int i = 0; i < nbTriangles; i++)
  {
    // -1 because Scilab's array indice starts at 1
    // second column
    firstPoints[i] = ((int) ppFec->pnoeud[i + nbTriangles]) - 1;
    // third column
    secondPoints[i] = ((int) ppFec->pnoeud[i +  2 * nbTriangles]) - 1;
    // fourth column
    thirdPoints[i] = ((int) ppFec->pnoeud[i + 3 * nbTriangles]) - 1;
  }

}
/*---------------------------------------------------------------------------------*/

}
