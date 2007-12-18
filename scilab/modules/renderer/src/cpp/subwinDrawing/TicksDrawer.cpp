/*------------------------------------------------------------------------*/
/* file: TicksDrawer.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks                              */
/*------------------------------------------------------------------------*/

#include "TicksDrawer.hxx"
#include "BasicAlgos.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawer::TicksDrawer(DrawableSubwin * subwin)
{
  m_pSubwin = subwin;
  m_pTicksComputer = NULL;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer::~TicksDrawer(void)
{
  if (m_pTicksComputer != NULL)
  {
    delete m_pTicksComputer;
    m_pTicksComputer = NULL;
  }
  m_pSubwin = NULL;
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
void TicksDrawer::draw(void)
{
  
  if (m_pTicksComputer == NULL) { return; }

  initializeDrawing();
  
  drawTicks();

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::showTicks(void)
{
  if (m_pTicksComputer == NULL) { return; }

  // same as initialize drawing but don't set constant parameters
  initializeShowing();
  
  drawTicks();

  endShowing();
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::drawTicks(void)
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

  // decimate ticks if needed
  if (m_pTicksComputer->needTicksDecimation())
  {
    while(!checkTicks(ticksPos, labels, labelsExponents, nbTicks))
    {
      m_pTicksComputer->reduceTicksNumber();
      // there is less ticks and positions, no need to reallocate smaller arrays

      // get new positions
      nbTicks = m_pTicksComputer->getNbTicks();
      m_pTicksComputer->getTicksPosition(ticksPos, labels, labelsExponents);
    }
  }

  // ticks are computed, now we can get subticks
  int nbSubticks = m_pTicksComputer->getNbSubticks(ticksPos, nbTicks);
  double * subticksPos = new double[nbSubticks];
  m_pTicksComputer->getSubticksPosition(ticksPos, nbTicks, subticksPos);

  // everything is computed so draw!!!
  drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);

  // clear used data
  if (m_pTicksComputer->isDisplayingLabelsExponents())
  {
    BasicAlgos::destroyStringArray(labelsExponents, initNbTicks);
  }

  BasicAlgos::destroyStringArray(labels, initNbTicks);

  delete[] ticksPos;
  delete[] subticksPos;
}
/*------------------------------------------------------------------------------------------*/
}
