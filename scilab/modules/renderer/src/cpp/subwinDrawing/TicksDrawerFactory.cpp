/*------------------------------------------------------------------------*/
/* file: TicksDrawerFactory.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for TicksDrawer classes                                 */
/*------------------------------------------------------------------------*/

#include "TicksDrawerFactory.hxx"
#include "XTicksDrawerJoGL.hxx"
#include "YTicksDrawerJoGL.hxx"
#include "UserDefinedTicksComputer.hxx"
#include "AutomaticTicksComputer.hxx"
#include "AutoLogTicksComputer.hxx"
#include "UserDefLogTicksComputer.hxx"

extern "C"
{
#include "GetProperty.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
TicksDrawerFactory::TicksDrawerFactory(DrawableSubwin * subwin)
{
  m_pDrawer = subwin;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerFactory::~TicksDrawerFactory(void)
{
  m_pDrawer = NULL;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer * TicksDrawerFactory::createXTicksDrawer(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();

  BOOL axesVisible[3];
  sciGetAxesVisible(pSubwin, axesVisible);
  if (!axesVisible[0]) {return NULL;}

  XTicksDrawerJoGL * newTicksDrawer = new XTicksDrawerJoGL(m_pDrawer);
  
  BOOL autoTicks[3];
  sciGetAutoTicks(pSubwin, autoTicks);

  char logFlags[3];
  sciGetLogFlags(pSubwin, logFlags);

  if (!autoTicks[0])
  {
    // ticks defines by user
    UserDefinedTicksComputer * ticksComputer = NULL;
    if (logFlags[0] == 'l')
    {
      ticksComputer = new UserDefLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new UserDefinedTicksComputer(m_pDrawer);
    }
    ticksComputer->setUserTicks(pSUBWIN_FEATURE(pSubwin)->axes.u_xgrads,
                                pSUBWIN_FEATURE(pSubwin)->axes.u_xlabels,
                                pSUBWIN_FEATURE(pSubwin)->axes.u_nxgrads,
                                pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[0]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }
  else
  {
    AutomaticTicksComputer * ticksComputer = NULL;
    if (logFlags[0] == 'l')
    {
      ticksComputer = new AutoLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new AutomaticTicksComputer(m_pDrawer);
    }
    
    new AutomaticTicksComputer(m_pDrawer);
    double bounds[6];
    sciGetDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[0], bounds[1]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  return newTicksDrawer;

}
/*------------------------------------------------------------------------------------------*/
TicksDrawer * TicksDrawerFactory::createYTicksDrawer(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();

  BOOL axesVisible[3];
  sciGetAxesVisible(pSubwin, axesVisible);
  if (!axesVisible[1]) {return NULL;}

  YTicksDrawerJoGL * newTicksDrawer = new YTicksDrawerJoGL(m_pDrawer);

  BOOL autoTicks[3];
  sciGetAutoTicks(pSubwin, autoTicks);

  char logFlags[3];
  sciGetLogFlags(pSubwin, logFlags);

  if (!autoTicks[1])
  {
    // ticks defines by user
    UserDefinedTicksComputer * ticksComputer = NULL;
    if (logFlags[1] == 'l')
    {
      ticksComputer = new UserDefLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new UserDefinedTicksComputer(m_pDrawer);
    }
    ticksComputer->setUserTicks(pSUBWIN_FEATURE(pSubwin)->axes.u_xgrads,
      pSUBWIN_FEATURE(pSubwin)->axes.u_ylabels,
      pSUBWIN_FEATURE(pSubwin)->axes.u_nygrads,
      pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[1]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }
  else
  {
    AutomaticTicksComputer * ticksComputer = NULL;
    if (logFlags[1] == 'l')
    {
      ticksComputer = new AutoLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new AutomaticTicksComputer(m_pDrawer);
    }

    new AutomaticTicksComputer(m_pDrawer);
    double bounds[6];
    sciGetDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[2], bounds[3]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  return newTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer * TicksDrawerFactory::createZTicksDrawer(void)
{
  return NULL;
}
/*------------------------------------------------------------------------------------------*/
}
