/*------------------------------------------------------------------------*/
/* file: XGridDrawerJoGL.hxx                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL for X axis         */
/*------------------------------------------------------------------------*/

#ifndef _X_GRID_DRAWER_JOGL_HXX_
#define _X_GRID_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "GridDrawerJoGL.hxx"

namespace sciGraphics
{

class XGridDrawerJoGL : public GridDrawerJoGL
{
public:

  XGridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~XGridDrawerJoGL(void);

protected:
  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);
};

}

#endif /* _X_GRID_DRAWER_JOGL_HXX_ */
