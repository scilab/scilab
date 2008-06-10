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

#ifndef _CAMERA_BRIDGE_FACTORY_HXX_
#define _CAMERA_BRIDGE_FACTORY_HXX_

#include "CameraBridge.h"
#include "CameraJoGL.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

class CameraBridgeFactory
{

public:

  CameraBridgeFactory( void ) { m_pCamera = NULL; }

  virtual ~CameraBridgeFactory( void ) { m_pCamera = NULL;}

  /**
   * Create a new cameraBridge with the right driver
   */
  CameraBridge * create( void ) ;

  /**
   * Update the camera bridge
   */
  void update(void);


  /**
   * Set the right java mapper
   */
  void setStrategies(CameraJoGL * cameraImp);

  /**
   * Set the parent subwin, needed by CameraImpFactory.
   */
  void setCorrespondingCamera( Camera * camera ) { m_pCamera = camera; }

protected:

  Camera * m_pCamera;

};

}

#endif /* _CAMERA_BRIDGE_FACTORY_HXX_ */
