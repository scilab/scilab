/*------------------------------------------------------------------------*/
/* file: CameraBridge.cpp                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Bridge between camera class and impelementation                 */
/*------------------------------------------------------------------------*/

#include "CameraBridge.h"
extern "C"
{
#include "BasicAlgos.h"
}


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
void CameraBridge::setRotationAngles( double alpha, double theta )
{
  m_dTheta = theta ;
  m_dAlpha = alpha ;
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setSubWinBox( double box[6] )
{
  doubleArrayCopy( m_aAxesBox, box, 6 );
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setFarthestDistance( double maxDist )
{
  m_dFarthestDist = maxDist;
}
/*-----------------------------------------------------------------------------------*/
void CameraBridge::setAxesBox( double bbox[6] )
{
  doubleArrayCopy(m_aAxesBox, bbox, 6) ;
}
/*-----------------------------------------------------------------------------------*/

}
