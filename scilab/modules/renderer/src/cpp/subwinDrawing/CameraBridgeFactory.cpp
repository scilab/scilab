/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for CameraBridge class
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CameraBridgeFactory.h"
#include "CameraJoGL.h"
#include "IsoViewCameraJavaMapper.hxx"
#include "IsometricCameraJavaMapper.hxx"
#include "getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
CameraBridge * CameraBridgeFactory::create( void )
{
  CameraJoGL * newBridge = new CameraJoGL(m_pCamera) ;

  setStrategies(newBridge);
  
  return newBridge;
}
/*------------------------------------------------------------------------*/
void CameraBridgeFactory::update(void)
{
  DrawableObjectBridge * curCameraBridge = getSubwinDrawer(m_pCamera->getDrawedObject())->getCamera()->getDrawableImp();

  setStrategies(dynamic_cast<CameraJoGL *>(curCameraBridge));
}
/*------------------------------------------------------------------------*/
void CameraBridgeFactory::setStrategies(CameraJoGL * cameraImp)
{
  sciPointObj * pSubwin = cameraImp->getDrawer()->getDrawedObject();
  CameraJavaMapper * javaMapper = NULL;

  if (sciGetIsIsoView(pSubwin))
  {
    javaMapper = new IsoViewCameraJavaMapper() ;
  }
  else
  {
    javaMapper = new IsometricCameraJavaMapper() ;
  }

  cameraImp->setJavaMapper(javaMapper);

}
/*------------------------------------------------------------------------*/

}
