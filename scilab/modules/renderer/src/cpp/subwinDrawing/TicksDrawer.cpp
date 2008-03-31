/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing ticks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TicksDrawer.hxx"
#include "BasicAlgos.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawer::TicksDrawer(DrawableObject * drawer)
{
  m_pTicksComputer = NULL;
  m_pGridDrawer = NULL;
  m_pTicksPositioner = NULL;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer::~TicksDrawer(void)
{
  setTicksComputer(NULL);
  setGridDrawer(NULL);
  setTicksPositioner(NULL);
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::setTicksComputer(ComputeTicksStrategy * ticksComputer)
{
  if (m_pTicksComputer != NULL)
  {
    delete m_pTicksComputer;
  }
  m_pTicksComputer = ticksComputer;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::setTicksPositioner(PlaceTicksStrategy * ticksPositioner)
{
  if (m_pTicksPositioner != NULL)
  {
    delete m_pTicksPositioner;
  }
  m_pTicksPositioner = ticksPositioner;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::setGridDrawer(GridDrawer * gridDrawer)
{
  if (m_pGridDrawer != NULL)
  {
    delete m_pGridDrawer;
  }
  m_pGridDrawer = gridDrawer;
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawer::draw(void)
{
  initializeDrawing();
  
  double dist = drawTicks();

  endDrawing();

  return dist;
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawer::showTicks(void)
{

  // same as initialize drawing but don't set constant parameters
  //initializeShowing();
  initializeShowing();
  
  double dist = drawTicks();

  endShowing();

  return dist;
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawer::drawTicks(void)
{

  m_pTicksComputer->reinit();

  // allocate positions and ticks
  int initNbTicks = m_pTicksComputer->getNbTicks();
  char ** labels = BasicAlgos::createStringArray(initNbTicks);
  double * ticksPos = new double[initNbTicks];
  double * ticksPosX = new double[initNbTicks];
  double * ticksPosY = new double[initNbTicks];
  double * ticksPosZ = new double[initNbTicks];
  char ** labelsExponents = NULL;

  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    labelsExponents = BasicAlgos::createStringArray(initNbTicks);
  }

  // get ticks Abscissas
  m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);

  // convert them to 3D coordinates
  int nbTicksPos = initNbTicks; // final number of ticks
  int nbTicks;
  m_pTicksPositioner->computeTicksLocation(ticksPos, ticksPosX, ticksPosY, ticksPosZ,
                                           labels, labelsExponents, nbTicksPos, &nbTicks);

  setTicksPosition(ticksPosX, ticksPosY, ticksPosZ, nbTicks);
  setTicksLabels(labels, labelsExponents, nbTicks);

  // compute ticks direction and axis bounds
  double axisStart[3];
  double axisEnd[3];
  double ticksDirection[3];
  m_pTicksPositioner->computeAxisBounds(ticksPos, nbTicksPos, axisStart, axisEnd);
  m_pTicksPositioner->computeTicksDir(ticksDirection);
  setAxisPosition(axisStart, axisEnd, ticksDirection);


  // decimate ticks if needed
  if (m_pTicksComputer->needTicksDecimation())
  {
    while(!checkTicks())
    {
      m_pTicksComputer->reduceTicksNumber();
      // there is less ticks and positions, no need to reallocate smaller arrays

      // get new positions
      nbTicksPos = m_pTicksComputer->getNbTicks();
      m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);

      // convert them to 3D coordinates
      m_pTicksPositioner->computeTicksLocation(ticksPos, ticksPosX, ticksPosY, ticksPosZ,
                                               labels, labelsExponents, nbTicksPos, &nbTicks);


      setTicksPosition(ticksPosX, ticksPosY, ticksPosZ, nbTicks);
      setTicksLabels(labels, labelsExponents, nbTicks);
    }
  }

  // ticks are computed, now we can get subticks
  int nbSubticksPos = m_pTicksComputer->getNbSubticks(ticksPos, nbTicksPos);
  double * subticksPos = new double[nbSubticksPos];
  m_pTicksComputer->getSubticksPosition(ticksPos, nbTicksPos, subticksPos);


  double * subTicksPosX = new double[nbSubticksPos];
  double * subTicksPosY = new double[nbSubticksPos];
  double * subTicksPosZ = new double[nbSubticksPos];

  // convert them to 3D coordinates
  int nbSubticks;
  m_pTicksPositioner->computeTicksLocation(subticksPos, subTicksPosX, subTicksPosY, subTicksPosZ,
                                           NULL, NULL, nbSubticksPos, &nbSubticks);


  setSubticksPosition(subTicksPosX, subTicksPosY, subTicksPosZ, nbSubticks);


  // everything is computed so draw!!!
  double dist = concreteDrawTicks();

  // draw grid
  if (m_pGridDrawer != NULL)
  {
    drawGrid(ticksPos, subticksPos, nbTicksPos, nbSubticksPos);
  }

  // no longer needed
  // no longer needed
  delete[] subticksPos;
  subticksPos = NULL;
  delete[] ticksPos;
  ticksPos = NULL;

  // clear used data
  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    BasicAlgos::destroyStringArray(labelsExponents, initNbTicks);
  }

  BasicAlgos::destroyStringArray(labels, initNbTicks);
  
  delete[] ticksPosX;
  delete[] ticksPosY;
  delete[] ticksPosZ;
  delete[] subTicksPosX;
  delete[] subTicksPosY;
  delete[] subTicksPosZ;

  return dist;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::drawGrid(const double ticksPos[], const double subticksPos[],
                           int nbTicks, int nbSubticks)
{
  int nbGridLine;

  // alloczte start, middle and end coordinates
  double * startXPos = new double[nbTicks + nbSubticks];
  double * startYPos = new double[nbTicks + nbSubticks];
  double * startZPos = new double[nbTicks + nbSubticks];
  double * middleXPos = new double[nbTicks + nbSubticks];
  double * middleYPos = new double[nbTicks + nbSubticks];
  double * middleZPos = new double[nbTicks + nbSubticks];
  double * endXPos = new double[nbTicks + nbSubticks];
  double * endYPos = new double[nbTicks + nbSubticks];
  double * endZPos = new double[nbTicks + nbSubticks];

  // compute positions of points
  m_pTicksPositioner->computeGridStartPoints(ticksPos, subticksPos,
                                             startXPos, startYPos, startZPos,
                                             nbTicks, nbSubticks, &nbGridLine);
  m_pTicksPositioner->computeGridMiddlePoints(ticksPos, subticksPos,
                                              middleXPos, middleYPos, middleZPos,
                                              nbTicks, nbSubticks, &nbGridLine);
  m_pTicksPositioner->computeGridEndPoints(ticksPos, subticksPos,
                                           endXPos, endYPos, endZPos,
                                           nbTicks, nbSubticks, &nbGridLine);

  // give them to grid drawer
  m_pGridDrawer->setGridStartPoints(startXPos, startYPos, startZPos, nbGridLine);
  m_pGridDrawer->setGridMiddlePoints(middleXPos, middleYPos, middleZPos, nbGridLine);
  m_pGridDrawer->setGridEndPoints(endXPos, endYPos, endZPos, nbGridLine);

  // draw the grid
  m_pGridDrawer->drawGrid();


  // free temporary ressources
  delete[] startXPos;
  delete[] startYPos;
  delete[] startZPos;
  delete[] middleXPos;
  delete[] middleYPos;
  delete[] middleZPos;
  delete[] endXPos;
  delete[] endYPos;
  delete[] endZPos;
}
/*------------------------------------------------------------------------------------------*/
}
