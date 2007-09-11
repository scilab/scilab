/*------------------------------------------------------------------------*/
/* file: StairCaseDecomposition.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline to get data for bar drawing                  */
/*------------------------------------------------------------------------*/


#include "DecomposeLineStrategy.hxx"

#ifndef _BAR_DECOMPOSITION_HXX_
#define _BAR_DECOMPOSITION_HXX_

namespace sciGraphics
{

class BarDecomposition : public DecomposeLineStrategy
{
public:

  BarDecomposition( DrawablePolyline * polyline );

  virtual ~BarDecomposition( void );

  /**
   * Retrieve the vertices of the polyline which will be used to draw it directly.
   * In the polyline sciPointObj, x, y and z vectors does not always stands for the real vertices
   * of the drawn line (specially when staircase mode is on).
   * To allocate the array for retrieving vertices, user should use the getDrawnVerticesLength.
   * @param xCoords X coordinates of the drawn vertices
   * @param yCoords Y coordinates of the drawn vertices
   * @param zCoords Z coordinates of the drawn vertices
   */
  virtual void getDrawnVertices(double xCoords[], double yCoords[], double zCoords[]);

  /**
   * @return number of vertices which will be computed by the getDrawnVertices routine.
   */
  virtual int getDrawnVerticesLength(void);

  /**
   * Get the height of each bar to draw.
   * @param height array containg the height of size getDrawnVerticesLength().
   */
  void getBarHeight(double heights[]);

protected:

};

}

#endif /* _BAR_DECOMPOSITION_HXX_ */
