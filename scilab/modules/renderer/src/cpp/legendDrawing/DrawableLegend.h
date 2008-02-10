/*------------------------------------------------------------------------*/
/* file: DrawableLegend.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a legend object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LEGEND_H_
#define _DRAWABLE_LEGEND_H_


#include "../DrawableObject.h"

namespace sciGraphics
{

class DrawableLegend : public DrawableObject
{

public:

  DrawableLegend( sciPointObj * pObj ) : DrawableObject( pObj ) {}

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
   * Actually draw the legend
   */
  virtual void drawLegend(void) = 0;

  /**
   * Actually show the legend
   */
  virtual void showLegend(void) = 0;


} ;

}

#endif /* _DRAWABLE_LEGEND_H_  */
