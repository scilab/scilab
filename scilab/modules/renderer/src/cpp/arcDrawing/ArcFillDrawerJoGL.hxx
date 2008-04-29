/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy drawing the inside of an arc object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _ARC_FILL_DRAWER_JOGL_HXX_
#define _ARC_FILL_DRAWER_JOGL_HXX_

#include "DrawableArc.h"
#include "DrawArcStrategy.h"
#include "DrawableObjectJoGL.h"
#include "ArcFillDrawerJavaMapper.hxx"


namespace sciGraphics
{

/**
 * Strategy drawing the inside of an arc object
 */
class ArcFillDrawerJoGL : public DrawArcStrategy, public DrawableObjectJoGL
{

public:

  ArcFillDrawerJoGL( DrawableArc * arc ) ;

  virtual ~ArcFillDrawerJoGL(void);

  /**
   * Main algorithm
   */
  virtual void drawArc( void );

  /**
   * Recreate display list from precomputed data.
   */
  virtual void redrawArc(void);

  /**
   * Call the display list of an object
   */
  virtual void showArc( void );

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  ArcFillDrawerJavaMapper * getFillDrawerJavaMapper(void);


};

}

#endif /* _ARC_FILL_DRAWER_JOGL_HXX_ */

