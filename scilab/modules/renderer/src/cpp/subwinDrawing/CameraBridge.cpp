/*------------------------------------------------------------------------*/
/* file: CameraBridge.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Bridge between camera class and impelementation                 */
/*------------------------------------------------------------------------*/

#include "CameraBridge.h"

namespace sciGraphics
{

/*-----------------------------------------------------------------------------------*/
void CameraBridge::setViewingArea( double translation[2], double scale[2] )
{
  m_aViewingTranslation[0] = translation[0];
  m_aViewingTranslation[1] = translation[1];

  m_aViewingScale[0] = scale[0];
  m_aViewingScale[1] = scale[1];
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setAxesScale( double scale[3] )
{
  m_aAxesScale[0] = scale[0] ;
  m_aAxesScale[1] = scale[1] ;
  m_aAxesScale[2] = scale[2] ;
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setAxesTranslation( double translation[3] )
{
  m_aAxesTranslation[0] = translation[0] ;
  m_aAxesTranslation[1] = translation[1] ;
  m_aAxesTranslation[2] = translation[2] ;
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setAxesRotation( double alpha, double theta )
{
  m_dAlpha = alpha;
  m_dTheta = theta;
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setAxesCenter( double center[3] )
{
  m_aBoxCenter[0] = center[0];
  m_aBoxCenter[1] = center[1];
  m_aBoxCenter[2] = center[2];
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setFarthestDistance( double maxDist )
{
  m_dFarthestDist = maxDist;
}
/*-----------------------------------------------------------------------------------*/
}
