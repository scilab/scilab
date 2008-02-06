/*------------------------------------------------------------------------*/
/* file: DrawFecStrategy.hxx                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for drawing a fec object                               */
/*------------------------------------------------------------------------*/

#ifndef _FEC_DRAWING_STRATEGY_HXX_
#define _FEC_DRAWING_STRATEGY_HXX_

#include "DrawableFec.h"

namespace sciGraphics
{

class DrawFecStrategy
{
public:

  DrawFecStrategy(void) {}

  virtual ~DrawFecStrategy(void) {}

  /**
   * Draw the fec using this strategy
   * @param xCoords X coordinates of nodes
   * @param yCoords Y coordinates of nodes
   * @param values value for each node
   * @param nbNodes number of nodes (size of xCoords and yCoords).
   * @param firstPoints node indexes of triangles first point
   * @param seconfPoints node indexes of triangles second point
   * @param seconfPoints node indexes of triangles third point
   * @param nbTriangles number of triangle (size of first, second and thirdPoints).
   */
  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles) = 0;

  /**
   * Show the fec using this strategy
   */
  virtual void showFec(void) = 0;

protected:

  virtual DrawableFec * getFecDrawer(void) = 0;

};

}

#endif /* _FEC_DRAWING_STRATEGY_HXX_ */
