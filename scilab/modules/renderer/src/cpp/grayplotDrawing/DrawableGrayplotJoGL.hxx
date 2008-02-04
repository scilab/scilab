/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotJoGL.hxx                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        grayplot object with JoGL                                       */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_JOGL_HXX_
#define _DRAWABLE_GRAYPLOT_JOGL_HXX_


#include "DrawableGrayplotBridge.hxx"
#include "DrawableGrayplot.h"
#include "DrawableGrayplotJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

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

