/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for Camera
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CameraFactory.h"
#include "CameraBridgeFactory.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
Camera * CameraFactory::create( void )
{
  CameraBridgeFactory bf ;
  Camera * res = new Camera(m_pDrawed) ;

  bf.setCorrespondingCamera( res ) ;
  
  res->setDrawableImp( bf.create() ) ;

  return res;

}
/*------------------------------------------------------------------------*/
void CameraFactory::update( void )
{
  // replace the camera bridge

  // camera to update
  Camera * cam = getSubwinDrawer(m_pDrawed)->getCamera();
  CameraBridgeFactory bf ;
  bf.setCorrespondingCamera( cam ) ;
  bf.update();
}
/*------------------------------------------------------------------------*/
}
