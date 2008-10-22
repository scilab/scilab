/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 *  desc : Bridge between camera class and impelementation
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CameraBridge.h"

namespace sciGraphics
{

using namespace std;

/*--------------------------------------------------------------------------*/
CameraBridge::CameraBridge( void )
{
  
}
/*--------------------------------------------------------------------------*/
CameraBridge::~CameraBridge( void )
{
  
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
void CameraBridge::setAxesReverse(bool axisReverseX, bool axisReverseY, bool axisReverseZ)
{
  m_aAxesReverse[0] = axisReverseX;
  m_aAxesReverse[1] = axisReverseY;
  m_aAxesReverse[2] = axisReverseZ;
}
/*--------------------------------------------------------------------------*/

}
