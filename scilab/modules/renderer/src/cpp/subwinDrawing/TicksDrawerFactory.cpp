/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for TicksDrawer classes
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TicksDrawerFactory.hxx"
#include "XTicksDrawerJoGL.hxx"
#include "YTicksDrawerJoGL.hxx"
#include "ZTicksDrawerJoGL.hxx"
#include "UserDefinedTicksComputer.hxx"
#include "AutomaticTicksComputer.hxx"
#include "AutoLogTicksComputer.hxx"
#include "UserDefLogTicksComputer.hxx"
#include "XGridDrawerJoGL.hxx"
#include "YGridDrawerJoGL.hxx"
#include "ZGridDrawerJoGL.hxx"

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

  int xGridStyle;
  sciGetGridStyle(pSubwin, &xGridStyle, NULL, NULL);

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
    
    double bounds[6];
    sciGetDisplayedDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[0], bounds[1]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  if (xGridStyle >= 0)
  {
    // xgrid enable

    XGridDrawerJoGL * gridDrawer = new XGridDrawerJoGL(m_pDrawer);
    if (logFlags[0] == 'l')
    {
      gridDrawer->setLogMode(true);
    }
    else
    {
      gridDrawer->setLogMode(false);
    }
    newTicksDrawer->setGridDrawer(gridDrawer);
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

  int yGridStyle;
  sciGetGridStyle(pSubwin, NULL, &yGridStyle, NULL);

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
    ticksComputer->setUserTicks(pSUBWIN_FEATURE(pSubwin)->axes.u_ygrads,
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

    double bounds[6];
    sciGetDisplayedDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[2], bounds[3]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  if (yGridStyle >= 0)
  {
    // ygrid enable

    YGridDrawerJoGL * gridDrawer = new YGridDrawerJoGL(m_pDrawer);
    if (logFlags[1] == 'l')
    {
      gridDrawer->setLogMode(true);
    }
    else
    {
      gridDrawer->setLogMode(false);
    }
    newTicksDrawer->setGridDrawer(gridDrawer);
  }

  return newTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer * TicksDrawerFactory::createZTicksDrawer(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();

  BOOL axesVisible[3];
  sciGetAxesVisible(pSubwin, axesVisible);
  if (!axesVisible[2]) {return NULL;}

  // special case here, in 2D mode, Z axis is not displayed
  if (!sciGetIs3d(pSubwin))
  {
    return NULL;
  }

  ZTicksDrawerJoGL * newTicksDrawer = new ZTicksDrawerJoGL(m_pDrawer);

  BOOL autoTicks[3];
  sciGetAutoTicks(pSubwin, autoTicks);

  char logFlags[3];
  sciGetLogFlags(pSubwin, logFlags);

  int zGridStyle;
  sciGetGridStyle(pSubwin, NULL, NULL, &zGridStyle);

  if (!autoTicks[2])
  {
    // ticks defines by user
    UserDefinedTicksComputer * ticksComputer = NULL;
    if (logFlags[2] == 'l')
    {
      ticksComputer = new UserDefLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new UserDefinedTicksComputer(m_pDrawer);
    }
    ticksComputer->setUserTicks(pSUBWIN_FEATURE(pSubwin)->axes.u_zgrads,
                                pSUBWIN_FEATURE(pSubwin)->axes.u_zlabels,
                                pSUBWIN_FEATURE(pSubwin)->axes.u_nzgrads,
                                pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[2]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }
  else
  {
    AutomaticTicksComputer * ticksComputer = NULL;
    if (logFlags[2] == 'l')
    {
      ticksComputer = new AutoLogTicksComputer(m_pDrawer);
    }
    else
    {
      ticksComputer = new AutomaticTicksComputer(m_pDrawer);
    }

    double bounds[6];
    sciGetDisplayedDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[4], bounds[5]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  if (zGridStyle >= 0)
  {
    // ygrid enable

    ZGridDrawerJoGL * gridDrawer = new ZGridDrawerJoGL(m_pDrawer);
    if (logFlags[2] == 'l')
    {
      gridDrawer->setLogMode(true);
    }
    else
    {
      gridDrawer->setLogMode(false);
    }
    newTicksDrawer->setGridDrawer(gridDrawer);
  }

  return newTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
}
