/*------------------------------------------------------------------------*/
/* file: DrawableSegs.h                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a segs object          */
/*------------------------------------------------------------------------*/

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

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

  /**
   * Actually draw the segs
   */
  virtual void drawSegs(void) = 0;

  /**
   * Show the segs
   */
  virtual void showSegs(void) = 0;


} ;

}

#endif /* _DRAWABLE_SEGS_H_  */
