/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - INRIA - Pierre Lando
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
#include "BottomXAxisPositioner.hxx"
#include "TopXAxisPositioner.hxx"
#include "MiddleXAxisPositioner.hxx"
#include "OriginXAxisPositioner.hxx"
#include "RightYAxisPositioner.hxx"
#include "LeftYAxisPositioner.hxx"
#include "MiddleYAxisPositioner.hxx"
#include "OriginYAxisPositioner.hxx"
#include "ZAxisPositioner.hxx"
#include "UserDefinedTicksComputer.hxx"
#include "AutomaticTicksComputer.hxx"
#include "AutoLogTicksComputer.hxx"
#include "UserDefLogTicksComputer.hxx"
#include "UserDefLogSubticksComputer.hxx"
#include "UserDefinedSubticksComputer.hxx"
#include "AutomaticSubticksComputer.hxx"
#include "AutoLogSubticksComputer.hxx"
#include "XGridDrawerJoGL.hxx"
#include "YGridDrawerJoGL.hxx"
#include "ZGridDrawerJoGL.hxx"
#include "TicksDrawerJoGL.hxx"
#include "TicksDrawerJoGL.hxx"
#include "TicksDrawer.hxx"

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

  TicksDrawer * newTicksDrawer = new TicksDrawer();

  if (axesVisible[0])
  {
    newTicksDrawer->setTicksDrawer(new TicksDrawerJoGL(m_pDrawer));
  }
  
  BOOL autoTicks[3];
  sciGetAutoTicks(pSubwin, autoTicks);

  char logFlags[3];
  sciGetLogFlags(pSubwin, logFlags);

  int xGridStyle;
  sciGetGridStyle(pSubwin, &xGridStyle, NULL, NULL);

  // set ticks computer
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
    sciGetRealDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[0], bounds[1]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  // set subTicks computer
  newTicksDrawer->setSubticksComputer(createRightSubTicksComputer(m_pDrawer,
                                                                  sciGetAutoSubticks(pSubwin) == TRUE,
                                                                  logFlags[0],
                                                                  pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[0]));

  // set ticks positioner
  AxisPositioner * ticksPositioner = NULL;
  switch(pSUBWIN_FEATURE(pSubwin)->axes.xdir)
  {
  case 'u':
    ticksPositioner = new TopXAxisPositioner(m_pDrawer);
    break;
  case 'o':
    ticksPositioner = new OriginXAxisPositioner(m_pDrawer);
    break;
  case 'c':
    ticksPositioner = new MiddleXAxisPositioner(m_pDrawer);
    break;
  case 'd':
    ticksPositioner = new BottomXAxisPositioner(m_pDrawer);
    break;
  default:
    ticksPositioner = new TopXAxisPositioner(m_pDrawer);
    break;
  }

  newTicksDrawer->setAxisPositioner(ticksPositioner);


  // set grid drawer
  if (xGridStyle >= 0)
  {
    // xgrid enable

    XGridDrawerJoGL * gridDrawer = new XGridDrawerJoGL(m_pDrawer);
    if (logFlags[0] == 'l')
    {
      gridDrawer->drawSubticksGrid(true);
    }
    else
    {
      gridDrawer->drawSubticksGrid(false);
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
  

  TicksDrawer * newTicksDrawer = new TicksDrawer();

  if (axesVisible[1])
  {
    newTicksDrawer->setTicksDrawer(new TicksDrawerJoGL(m_pDrawer));
  }

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
    sciGetRealDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[2], bounds[3]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  // set subTicks computer
  newTicksDrawer->setSubticksComputer(createRightSubTicksComputer(m_pDrawer,
                                                                  sciGetAutoSubticks(pSubwin) == TRUE,
                                                                  logFlags[1],
                                                                  pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[1]));

  // set ticks positioner
  AxisPositioner * ticksPositioner = NULL;

  switch(pSUBWIN_FEATURE(pSubwin)->axes.ydir)
  {
  case 'l':
    ticksPositioner = new LeftYAxisPositioner(m_pDrawer);
    break;
  case 'c':
    ticksPositioner = new MiddleYAxisPositioner(m_pDrawer);
    break;
  case 'o':
    ticksPositioner = new OriginYAxisPositioner(m_pDrawer);
    break;
  case 'r':
    ticksPositioner = new RightYAxisPositioner(m_pDrawer);
    break;
  default:
    ticksPositioner = new LeftYAxisPositioner(m_pDrawer);
    break;
  }

  newTicksDrawer->setAxisPositioner(ticksPositioner);

  if (yGridStyle >= 0)
  {
    // ygrid enable

    YGridDrawerJoGL * gridDrawer = new YGridDrawerJoGL(m_pDrawer);
    if (logFlags[1] == 'l')
    {
      gridDrawer->drawSubticksGrid(true);
    }
    else
    {
      gridDrawer->drawSubticksGrid(false);
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

  // special case here, in 2D mode, Z axis is not displayed
  if (!sciGetIs3d(pSubwin))
  {
    return NULL;
  }

  TicksDrawer * newTicksDrawer = new TicksDrawer();

  if (axesVisible[2])
  {
    newTicksDrawer->setTicksDrawer(new TicksDrawerJoGL(m_pDrawer));
  }

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
    sciGetRealDataBounds(pSubwin, bounds);
    ticksComputer->setAxisBounds(bounds[4], bounds[5]);
    newTicksDrawer->setTicksComputer(ticksComputer);
  }

  // set subTicks computer
  newTicksDrawer->setSubticksComputer(createRightSubTicksComputer(m_pDrawer,
                                                                  sciGetAutoSubticks(pSubwin) == TRUE,
                                                                  logFlags[2],
                                                                  pSUBWIN_FEATURE(pSubwin)->axes.nbsubtics[2]));

  // set ticks positioner

  newTicksDrawer->setAxisPositioner(new ZAxisPositioner(m_pDrawer));

  if (zGridStyle >= 0)
  {
    // ygrid enable

    ZGridDrawerJoGL * gridDrawer = new ZGridDrawerJoGL(m_pDrawer);
    if (logFlags[2] == 'l')
    {
      gridDrawer->drawSubticksGrid(true);
    }
    else
    {
      gridDrawer->drawSubticksGrid(false);
    }
    newTicksDrawer->setGridDrawer(gridDrawer);
  }

  return newTicksDrawer;
}
/*------------------------------------------------------------------------------------------*/
ComputeSubticksStrategy * TicksDrawerFactory::createRightSubTicksComputer(DrawableSubwin * subwin,
                                                                          bool isAuto,
                                                                          char logFlag,
                                                                          int defaultNbSubticks)
{
  if (isAuto)
  {
    if (logFlag == 'l')
    {
      return new AutoLogSubticksComputer(subwin);
    }
    else
    {
      return new AutomaticSubticksComputer(subwin);
    }
  }
  else
  {
    UserDefinedSubticksComputer * res;
    if (logFlag == 'l')
    {
      res = new UserDefLogSubticksComputer(subwin);
    }
    else
    {
      res = new UserDefinedSubticksComputer(subwin);
    }
    res->setUserSubticks(defaultNbSubticks);
    return res;
  }

}
/*------------------------------------------------------------------------------------------*/
}
