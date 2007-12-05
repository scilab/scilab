/*------------------------------------------------------------------------*/
/* file: BackTrihedronDrawerJoGL.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing only the backTrihedron of a subwin object         */
/*------------------------------------------------------------------------*/

#ifndef _BACK_TRIHEDRON_DRAWER_HXX_
#define _BACK_TRIHEDRON_DRAWER_HXX_

#include "DrawAxesBoxStrategy.hxx"
#include "../DrawableObjectJoGL.h"
#include "BackTrihedronDrawerJavaMapper.hxx"

namespace sciGraphics
{

class BackTrihedronDrawerJoGL: public DrawAxesBoxStrategy, public DrawableObjectJoGL
{
public:
  
   BackTrihedronDrawerJoGL(DrawableSubwin * subwin);

   virtual ~BackTrihedronDrawerJoGL(void);

   /**
    * Main funtion which actually draw the box.
    */
   virtual void drawAxesBox(void);

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  BackTrihedronDrawerJavaMapper * getBackTrihedronDrawerJavaMapper(void);

};

}

#endif /* _BACK_TRIHEDRON_DRAWER_HXX_ */
