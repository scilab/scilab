/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableArc.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableArc.h"

extern "C"
{
#include "core_math.h"
}

using namespace std;

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
ConcreteDrawableArc::ConcreteDrawableArc(sciPointObj * pObj) : DrawableArc(pObj)
{
  m_oDrawingStrategies.clear();
}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawableArc::~ConcreteDrawableArc(void)
{
  removeDrawingStrategies();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableArc::addDrawingStrategy( DrawArcStrategy * strategy )
{
  m_oDrawingStrategies.push_back(strategy);
}
/*------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------*/
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

  center[0] = ppArc->x + ppArc->width / 2.0 ;
  center[1] = ppArc->y - ppArc->height / 2.0 ;
  center[2] = ppArc->z ;

  if ( Abs(ppArc->width) >= Abs(ppArc->height) )
  {
    semiMinorAxis[0] = 0.0;
    semiMinorAxis[1] = -ppArc->height / 2.0;
    semiMinorAxis[2] = 0.0;

    semiMajorAxis[0] = ppArc->width / 2.0;
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
    semiMinorAxis[0] = ppArc->width / 2.0;
    semiMinorAxis[1] = 0.0;
    semiMinorAxis[2] = 0.0;

    semiMajorAxis[0] = 0.0;
    semiMajorAxis[1] = ppArc->height / 2.0;
    semiMajorAxis[2] = 0.0;
    startAngle = Min(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
    endAngle   = Max(ppArc->alphabegin, ppArc->alphabegin + ppArc->alphaend);
  }

  

}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableArc::drawArc(void)
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawArc();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawableArc::showArc(void)
{
  list<DrawArcStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->showArc();
  }
}
/*------------------------------------------------------------------------------------------*/
}
