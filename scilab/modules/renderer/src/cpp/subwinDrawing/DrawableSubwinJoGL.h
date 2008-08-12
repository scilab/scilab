/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a
 * subwin object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SUBWIN_JOGL_H_
#define _DRAWABLE_SUBWIN_JOGL_H_

#include "DrawableSubwinBridge.h"
#include "DrawableSubwin.h"
#include "../DrawableObjectJoGL.h"
#include "DrawableSubwinJavaMapper.hxx"

namespace sciGraphics
{

  class DrawableSubwinJoGL : public virtual DrawableSubwinBridge, public DrawableObjectJoGL
  {

  public:

    DrawableSubwinJoGL( DrawableSubwin * drawer ) ;

    virtual ~DrawableSubwinJoGL( void ) ;

    /**
     * Initialize the context for drawing (incl starting recording display list)
     */
    virtual void initializeDrawing( void ) ;

    /**
     * Return the drawn object
     */
    virtual DrawableSubwin * getSubwinDrawer( void ) ;

  protected:

    /*----------------------------------------------------------------*/
    /**
     * Get the object performing mapping with Java class.
     */
    DrawableSubwinJavaMapper * getSubwinJavaMapper(void);
    /*----------------------------------------------------------------*/

  } ;

}

#endif /* _DRAWABLE_SUBWIN_JOGL_H_  */
