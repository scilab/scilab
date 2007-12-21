/*------------------------------------------------------------------------*/
/* file: YGridDrawerJoGL.hxx                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for Y axis         */
/*------------------------------------------------------------------------*/

#ifndef _Y_GRID_DRAWER_JOGL_HXX_
#define _Y_GRID_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "GridDrawerJoGL.hxx"

namespace sciGraphics
{

class YGridDrawerJoGL : public GridDrawerJoGL
{
public:

  YGridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~YGridDrawerJoGL(void);

protected:
  /**
  * Specify initialize drawing in setting all useful parameters.
  */
  virtual void initializeDrawing(void);
};

}

#endif /* _Y_GRID_DRAWER_JOGL_HXX_ */
