/*------------------------------------------------------------------------*/
/* file: CameraBridge.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Bridge between camera class and impelementation                 */
/*------------------------------------------------------------------------*/

#include "CameraBridge.h"

namespace sciGraphics
{

using namespace std;

/*--------------------------------------------------------------------------*/
CameraBridge::CameraBridge( void )
{
  m_oReverseStrategies.clear();
}
/*--------------------------------------------------------------------------*/
CameraBridge::~CameraBridge( void )
{
  removeAxesReverseStrategies();
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setViewingArea( double translation[2], double scale[2] )
{
  m_aViewingTranslation[0] = translation[0];
  m_aViewingTranslation[1] = translation[1];

  m_aViewingScale[0] = scale[0];
  m_aViewingScale[1] = scale[1];
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setAxesFittingScale( double scale[3] )
{
  m_aAxesFittingScale[0] = scale[0] ;
  m_aAxesFittingScale[1] = scale[1] ;
  m_aAxesFittingScale[2] = scale[2] ;
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setAxesTranslation( double translation[3] )
{
  m_aAxesTranslation[0] = translation[0] ;
  m_aAxesTranslation[1] = translation[1] ;
  m_aAxesTranslation[2] = translation[2] ;
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setAxesRotation( double alpha, double theta )
{
  m_dAlpha = alpha;
  m_dTheta = theta;
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setAxesCenter( double center[3] )
{
  m_aBoxCenter[0] = center[0];
  m_aBoxCenter[1] = center[1];
  m_aBoxCenter[2] = center[2];
}
/*--------------------------------------------------------------------------*/
void CameraBridge::setAxesNormalizationScale(double scale[3])
{
  m_aAxesNormalizationScale[0] = scale[0];
  m_aAxesNormalizationScale[1] = scale[1];
  m_aAxesNormalizationScale[2] = scale[2];
}
/*--------------------------------------------------------------------------*/
void CameraBridge::addAxesReverseStrategy(AxesReverseStrategy * strategy)
{
  m_oReverseStrategies.push_back(strategy);
}
/*--------------------------------------------------------------------------*/
void CameraBridge::removeAxesReverseStrategies(void)
{
  list<AxesReverseStrategy *>::iterator it = m_oReverseStrategies.begin();
  for( ; it != m_oReverseStrategies.end(); it++)
  {
    delete *it;
    *it = NULL;
  }
  m_oReverseStrategies.clear();
}
/*--------------------------------------------------------------------------*/
void CameraBridge::revertAxes(void)
{
  list<AxesReverseStrategy *>::iterator it = m_oReverseStrategies.begin();
  for( ; it != m_oReverseStrategies.end(); it++)
  {
    (*it)->revertAxis();
  }
}
/*--------------------------------------------------------------------------*/

}
