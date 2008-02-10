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
  double dist = drawTicks(ticksPos, labels, labelsExponents, nbTicks, subticksPos, nbSubticks);

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
