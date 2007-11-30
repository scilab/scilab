/*------------------------------------------------------------------------*/
/* file: LinearBarDecomposition.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline to get data for bar drawing in linear mode   */
/*------------------------------------------------------------------------*/

#ifndef _LINEAR_BAR_DECOMPOSITION_HXX_
#define _LINEAR_BAR_DECOMPOSITION_HXX_

#include "DrawableObject.h"

namespace sciGraphics
{

class BarDecomposition;

class LinearBarDecomposition
{
public:

  LinearBarDecomposition(BarDecomposition * decomposition) {m_pDecomposition = decomposition;}

  virtual ~LinearBarDecomposition(void) {m_pDecomposition = NULL;}

  /**
   * Get the top and bottom ordinates of each bar in linear coordinates.
   */
  virtual void getBarOrdinates(double bottom[], double top[]) = 0;

  /**
   * Get the left and right abscissas of each bar in linear coordinates.
   */
  virtual void getBarAbscissas(double left[], double right[]) = 0;

protected:

  BarDecomposition * m_pDecomposition;

};

}

#endif /* _LINEAR_BAR_DECOMPOSITION_HXX_ */
