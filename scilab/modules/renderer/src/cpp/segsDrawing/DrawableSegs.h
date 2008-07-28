/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a segs object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SEGS_H_
#define _DRAWABLE_SEGS_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableSegs : public DrawableClippedObject
{

public:

  DrawableSegs( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Compute the postions of the arraows to display
   */
  virtual void getSegsPos(double startXcoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[],
                          double startZCoords[], double endZcoords[]) = 0;

  /**
   * Get the number of arrows in the segs or champ object
   */
  virtual int getNbSegment(void) = 0;

  /**
   * To know if each segs object has a distinct color
   */
  virtual bool isColored(void) = 0;

  /**
   * Get the color of each segement if needed
   */
  virtual void getSegsColors(int colors[]) = 0;

  /**
   * Compute the bounding box a segs object
   * Used to set the subwin size accordingly
   * @param bounds [xmin, xmax, ymin, ymax, zmin, zmax]
   */
  virtual void getBoundingBox(double bounds[6]) = 0;

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
   * Redraw the object using precomputed data.
   */
  virtual EDisplayStatus redraw(void);

  /**
   * Actually draw the segs
   */
  virtual EDisplayStatus drawSegs(void) = 0;

  /**
   * Show the segs
   */
  virtual void showSegs(void) = 0;

  /**
   * Redraw the segs
   */
  virtual void redrawSegs(void) = 0;


} ;

}

#endif /* _DRAWABLE_SEGS_H_  */
