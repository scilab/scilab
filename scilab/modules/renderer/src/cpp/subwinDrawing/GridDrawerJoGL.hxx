/*------------------------------------------------------------------------*/
/* file: GridDrawerJoGL.hxx                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL                    */
/*------------------------------------------------------------------------*/

#ifndef _GRID_DRAWER_JOGL_HXX_
#define _GRID_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "GridDrawer.hxx"
#include "GridDrawerJavaMapper.hxx"

namespace sciGraphics
{

class GridDrawerJoGL : public GridDrawer, public DrawableObjectJoGL
{
public:

  GridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~GridDrawerJoGL(void);

protected:

  /**
   * Draw the grid
   */
  void drawGrid(const double gridPositions[], int nbPositions);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);

  /**
   * Specify end drawing in setting all useful parameters.
   */
  virtual void endDrawing(void);

  /**
   * Same as initialize drawing but doesn't set constant parameters
   */
  virtual void initializeShowing(void);

  /**
   * Same as end drawing but doesn't set constant parameters
   */
  virtual void endShowing(void);

  /**
   * Get the object performing mapping with Java class.
   */
  GridDrawerJavaMapper * getGridDrawerJavaMapper(void);

};

}

#endif /* _GRID_DRAWER_JOGL_HXX_ */
