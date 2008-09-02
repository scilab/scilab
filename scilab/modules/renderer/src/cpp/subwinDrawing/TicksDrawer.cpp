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

#include <cstdio>

#include "TicksDrawer.hxx"
#include "BasicAlgos.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawer::TicksDrawer(void)
{
  m_pTicksComputer = NULL;
  m_pSubticksComputer = NULL;
  m_pGridDrawer = NULL;
  m_pPositioner = NULL;
  m_pTicksDrawer = NULL;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer::~TicksDrawer(void)
{
  setTicksComputer(NULL);
  setSubticksComputer(NULL);
  setGridDrawer(NULL);
  setAxisPositioner(NULL);
  setTicksDrawer(NULL);
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
void TicksDrawer::setSubticksComputer(ComputeSubticksStrategy * subticksComputer)
{
  if (m_pSubticksComputer != NULL)
  {
    delete m_pSubticksComputer;
  }
  m_pSubticksComputer = subticksComputer;
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
void TicksDrawer::setAxisPositioner(AxisPositioner * positioner)
{
  if (m_pPositioner != NULL)
  {
    delete m_pPositioner;
  }
  m_pPositioner = positioner;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::setTicksDrawer(TicksDrawerBridge * drawer)
{
  if (m_pTicksDrawer != NULL)
  {
    delete m_pTicksDrawer;
  }
  m_pTicksDrawer = drawer;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::initializeDrawing(void)
{
  if (m_pTicksDrawer != NULL)
  {
    m_pTicksDrawer->initializeDrawing();
    m_pTicksDrawer->setNeedTicksDecimation(m_pTicksComputer->needTicksDecimation());
  }

  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->initializeDrawing();
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::endDrawing(void)
{
  if (m_pTicksDrawer != NULL)
  {
    m_pTicksDrawer->endDrawing();
  }

  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->endDrawing();
  }
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
double TicksDrawer::show(void)
{
  initializeDrawing();
  
  double dist = showTicks();

  endDrawing();

  return dist;
}
/*------------------------------------------------------------------------------------------*/
int TicksDrawer::getInitNbTicks(void)
{
  m_pTicksComputer->reinit();
  return m_pTicksComputer->getNbTicks();
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::getInitTicksPos(double ticksPositions[], char ** ticksLabels)
{
  m_pTicksComputer->reinit();
  

  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    int nbTicks = m_pTicksComputer->getNbTicks();
    char ** labels = BasicAlgos::createStringArray(nbTicks);
    char ** labelsExponents = BasicAlgos::createStringArray(nbTicks);

    // get positions, labels and exponents
    m_pTicksComputer->getTicksPosition(ticksPositions, labels, labelsExponents);

    // concatenate labels and exponents
    for (int i = 0; i < nbTicks; i++)
    {
      int labelLength = (int)strlen(labels[i]);
      int exponentLength = (int)strlen(labelsExponents[i]);

      // +1 for the e and +1 for the null terminating character
      if (ticksLabels[i] != NULL)
      {
        delete[] ticksLabels[i];
      }
      ticksLabels[i] = new char[labelLength + exponentLength + 2];

      // copy all inside res
      sprintf(ticksLabels[i], "%se%s", labels[i], labelsExponents[i]);
    }

    BasicAlgos::destroyStringArray(labelsExponents, nbTicks);
    BasicAlgos::destroyStringArray(labels, nbTicks);

  }
  else
  {
    // no exponents, everything is in labels
    m_pTicksComputer->getTicksPosition(ticksPositions, ticksLabels, NULL);
  }

}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::getAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3])
{
  m_pPositioner->getAxisBounds(axisStart, axisEnd);
  m_pPositioner->getTicksDirection(ticksDirection);
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawer::drawTicks(void)
{

  // first get axis position
  double axisSegmentStart[3];
  double axisSegmentEnd[3];
  double ticksDirection[3];
  m_pPositioner->getAxisBounds(axisSegmentStart, axisSegmentEnd);
  m_pPositioner->getTicksDirection(ticksDirection);

  // then compute ticks positions
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
  int initNbSubticks = m_pSubticksComputer->getNbSubticks(ticksPos, nbTicks);
  double * subticksPos = new double[initNbSubticks];
  m_pSubticksComputer->getSubticksPosition(ticksPos, nbTicks, subticksPos);
  int nbSubticks = initNbSubticks;

  // get relative ticks positions
  m_pPositioner->getRelativeTicksPosition(ticksPos, nbTicks);
  m_pPositioner->getRelativeTicksPosition(subticksPos, nbSubticks);
  

  // decimate ticks if needed
  double dist = 0.0;
  if (m_pTicksDrawer != NULL)
  {
    dist = m_pTicksDrawer->drawTicks(ticksPos, labels, labelsExponents, nbTicks,
                                     subticksPos, nbSubticks,
                                     axisSegmentStart, axisSegmentEnd, ticksDirection);

    if (m_pTicksComputer->needTicksDecimation())
    {
      while(dist < 0.0)
      {
        m_pTicksComputer->reduceTicksNumber();
        // there is less ticks and positions, no need to reallocate smaller arrays

        // get new positions
        nbTicks = m_pTicksComputer->getNbTicks();
        m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);

        nbSubticks = m_pSubticksComputer->getNbSubticks(ticksPos, nbTicks);
        // unfortunately subticks numbers may increase
        // so somtime we need to reallocate subticks
        if (nbSubticks > initNbSubticks)
        {
          delete[] subticksPos;
          subticksPos = new double[nbSubticks];
          initNbSubticks = nbSubticks;
        }
        m_pSubticksComputer->getSubticksPosition(ticksPos, nbTicks, subticksPos);

         // get relative ticks positions
        m_pPositioner->getRelativeTicksPosition(ticksPos, nbTicks);
        m_pPositioner->getRelativeTicksPosition(subticksPos, nbSubticks);

        dist = m_pTicksDrawer->drawTicks(ticksPos, labels, labelsExponents, nbTicks,
                                         subticksPos, nbSubticks,
                                         axisSegmentStart, axisSegmentEnd, ticksDirection);

      }
    }
  }

  // everything is computed so draw!!!
  //double dist = drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);

  // draw grid
  if (m_pGridDrawer != NULL)
  {
    double firstAxisStart[3];
    double firstAxisEnd[3];
    double secondAxisStart[3];
    double secondAxisEnd[3];
    double thirdAxisStart[3];
    double thirdAxisEnd[3];
    m_pPositioner->getGridEdges(firstAxisStart, firstAxisEnd, secondAxisStart, secondAxisEnd, thirdAxisStart, thirdAxisEnd);

    m_pGridDrawer->draw(firstAxisStart, firstAxisEnd, secondAxisStart, secondAxisEnd, thirdAxisStart, thirdAxisEnd,
                        ticksPos, nbTicks, subticksPos, nbSubticks);
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
double TicksDrawer::showTicks(void)
{
  double res = 0.0;
  if (m_pTicksDrawer != NULL)
  {
    res = m_pTicksDrawer->showTicks();
  }

  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->show();
  }
  return res;
}
/*------------------------------------------------------------------------------------------*/

}
