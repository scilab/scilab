/*------------------------------------------------------------------------*/
/* file: ZGridDrawerJoGL.hxx                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for Z axis         */
/*------------------------------------------------------------------------*/

#ifndef _Z_GRID_DRAWER_JOGL_HXX_
#define _Z_GRID_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "GridDrawerJoGL.hxx"

namespace sciGraphics
{

class ZGridDrawerJoGL : public GridDrawerJoGL
{
public:

  ZGridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~ZGridDrawerJoGL(void);

protected:
  /**
  * Specify initialize drawing in setting all useful parameters.
  */
  virtual void initializeDrawing(void);
};

}

#endif /* _X_GRID_DRAWER_JOGL_HXX_ */
