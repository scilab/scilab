/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotBridge.hxx                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        grayplot object                                                 */
/*------------------------------------------------------------------------*/

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
