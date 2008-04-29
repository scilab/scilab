/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing implementation of driver independent algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableArc.h"

extern "C"
{
#include "core_math.h"
}

using namespace std;

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
ConcreteDrawableArc::ConcreteDrawableArc(sciPointObj * pObj) : DrawableArc(pObj)
{
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableArc::~ConcreteDrawableArc(void)
{
  removeDrawingStrategies();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::addDrawingStrategy( DrawArcStrategy * strategy )
{
  m_oDrawingStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::removeDrawingStrategies( void )
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::getArcRepresentation(double center[3], double semiMinorAxis[3], double semiMajorAxis[3],
                                               double & startAngle, double & endAngle)
{ //    <---- w ---->
  // (x,y)-----------+
  //   |      ^      |
  //   |   smi|      |
  // h |      C----->|
  //   |        sma  |
  //   |             |
  //   +-------------+


  sciArc * ppArc = pARC_FEATURE(m_pDrawed) ;

  double width;
  double height;
  double x;
  double y;
  double z;

  // apply log scale if needed
  // get lower x and y for that
  pointScale(ppArc->x, ppArc->y - ppArc->height, ppArc->z, &x, &y, &z);
  directionScale(ppArc->width, ppArc->height, 0.0,
                 ppArc->x, ppArc->y - ppArc->height, ppArc->z,
                 &width, &height, NULL);

  center[0] = x + width / 2.0 ;
  center[1] = y + height / 2.0 ;
  center[2] = z ;

  

  if ( Abs(width) >= Abs(height) )
  {
    semiMinorAxis[0] = 0.0;
    semiMinorAxis[1] = -height / 2.0;
    semiMinorAxis[2] = 0.0;

    semiMajorAxis[0] = width / 2.0;
    semiMajorAxis[1] = 0.0;
    semiMajorAxis[2] = 0.0;

    startAngle = Min(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
    endAngle   = Max(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
    startAngle += M_PI / 2.0;
    endAngle   += M_PI / 2.0;
  }
  else
  {
    // rotate of -Pi/2 from previous case
    semiMinorAxis[0] = width / 2.0;
    semiMinorAxis[1] = 0.0;
    semiMinorAxis[2] = 0.0;

    semiMajorAxis[0] = 0.0;
    semiMajorAxis[1] = height / 2.0;
    semiMajorAxis[2] = 0.0;
    startAngle = Min(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
    endAngle   = Max(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
  }

  

}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::drawArc(void)
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawArc();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::redrawArc(void)
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->redrawArc();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableArc::showArc(void)
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->showArc();
  }
}
/*---------------------------------------------------------------------------------*/
}
