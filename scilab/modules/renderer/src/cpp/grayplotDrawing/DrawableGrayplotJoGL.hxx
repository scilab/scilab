/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependant routines to draw a 
 * grayplot object with JoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_GRAYPLOT_JOGL_HXX_
#define _DRAWABLE_GRAYPLOT_JOGL_HXX_


#include "DrawableGrayplotBridge.hxx"
#include "DrawableGrayplot.h"
#include "DrawableGrayplotJavaMapper.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableGrayplotJoGL : public DrawableGrayplotBridge, public DrawableClippedObjectJoGL
{

public:

  DrawableGrayplotJoGL( DrawableGrayplot * drawer ) ;

  virtual ~DrawableGrayplotJoGL( void ) ;


  /**
   * Draw the grayplot using precomputed positions and colors
   */
  virtual void drawGrayplot(const double xGrid[], int nbRow,
                            const double yGrid[], int nbCol,
                            double zCoord, const int colors[],
                            int nbColors);

  /**
   * Return the drawn object
   */
  DrawableGrayplot * getGrayplotDrawer( void ) ;

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  DrawableGrayplotJavaMapper * getGrayplotJavaMapper(void);

} ;

}

#endif /* _DRAWABLE_GRAYPLOT_JOGL_H_  */

