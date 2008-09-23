/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a fec object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_FEC_H_
#define _DRAWABLE_FEC_H_


#include "DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableFec : public DrawableClippedObject
{

public:

  DrawableFec( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Actually draw the fec objets
   */
  virtual EDisplayStatus drawFec(void) = 0;

  /**
   * Actually show fec object
   */
  virtual void showFec(void) = 0;

} ;

}

#endif /* _DRAWABLE_FEC_H_  */
