/*------------------------------------------------------------------------*/
/* file: StairCaseDecomposition.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline with creating a stair case between each      */
/*        data.                                                           */
/*------------------------------------------------------------------------*/

#include "DecomposeLineStrategy.hxx"

#ifndef _STAIR_CASE_DECOMPOSITION_HXX_
#define _STAIR_CASE_DECOMPOSITION_HXX_

namespace sciGraphics
{

class StairCaseDecomposition : public DecomposeLineStrategy
{
public:

  StairCaseDecomposition( DrawablePolyline * polyline );

  virtual ~StairCaseDecomposition( void );

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

protected:

};

}

#endif /* _STAIR_CASE_DECOMPOSITION_HXX_ */
