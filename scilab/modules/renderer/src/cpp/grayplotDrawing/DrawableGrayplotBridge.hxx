/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver dependent routines to draw a
 * grayplot object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_GRAYPLOT_BRIDGE_HXX_
#define _DRAWABLE_GRAYPLOT_BRIDGE_HXX_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"


namespace sciGraphics
{

/** To avoid cyclic dependencies */
class DrawableGrayplot;

class DrawableGrayplotBridge : public virtual DrawableObjectBridge
{
public:

  DrawableGrayplotBridge( void ) {}

  virtual ~DrawableGrayplotBridge( void ) {}

  /**
   * Draw the grayplot using precomputed positions and colors
   */
  virtual void drawGrayplot(const double xGrid[], int nbRow,
                            const double yGrid[], int nbCol,
                            double zCoord, const int colors[],
                            int nbColors) = 0;

  /**
   * Return the drawn object
   */
  virtual DrawableGrayplot * getGrayplotDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_GRAYPLOT_BRIDGE_HXX_ */

