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
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer::~TicksDrawer(void)
{
  setTicksComputer(NULL);
  setGridDrawer(NULL);
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
  char ** labelsExponents = NULL;

  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    labelsExponents = BasicAlgos::createStringArray(initNbTicks);
  }

  m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);
  // final number of ticks
  int nbTicks = initNbTicks;

  // ticks are computed, now we can get subticks
  int initNbSubticks = m_pTicksComputer->getNbSubticks(ticksPos, nbTicks);
  double * subticksPos = new double[initNbSubticks];
  m_pTicksComputer->getSubticksPosition(ticksPos, nbTicks, subticksPos);
  int nbSubticks = initNbSubticks;
  

  // decimate ticks if needed
  double dist = drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);
  if (m_pTicksComputer->needTicksDecimation())
  {
    while(dist < 0.0)
    {
      m_pTicksComputer->reduceTicksNumber();
      // there is less ticks and positions, no need to reallocate smaller arrays

      // get new positions
      nbTicks = m_pTicksComputer->getNbTicks();
      m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);

      nbSubticks = m_pTicksComputer->getNbSubticks(ticksPos, nbTicks);
      // unfortunately subticks numbers may increase
      // so somtime we need to reallacote subticks
      if (nbSubticks > initNbSubticks)
      {
        delete[] subticksPos;
        subticksPos = new double[nbSubticks];
        initNbSubticks = nbSubticks;
      }
      m_pTicksComputer->getSubticksPosition(ticksPos, nbTicks, subticksPos);

      dist = drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);

    }
  }

  // everything is computed so draw!!!
  //double dist = drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);

  // draw grid
  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->draw(ticksPos, nbTicks, subticksPos, nbSubticks);
  }

  // clear used data
  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    BasicAlgos::destroyStringArray(labelsExponents, initNbTicks);
  }

  BasicAlgos::destroyStringArray(labels, initNbTicks);

  delete[] ticksPos;
  delete[] subticksPos;

  return dist;
}
/*------------------------------------------------------------------------------------------*/
}
