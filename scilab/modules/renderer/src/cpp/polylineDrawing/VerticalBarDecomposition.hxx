/*------------------------------------------------------------------------*/
/* file: VerticalBarDecomposition.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline to get data for vertical bar drawing         */
/*------------------------------------------------------------------------*/

#ifndef _VERTICAL_BAR_DECOMPOSITION_HXX_
#define _VERTICAL_BAR_DECOMPOSITION_HXX_

#include "LinearBarDecomposition.hxx"
#include "BarDecomposition.hxx"

namespace sciGraphics
{

class VerticalBarDecomposition : public LinearBarDecomposition
{
public:

  VerticalBarDecomposition( BarDecomposition * decomposition );

  virtual ~VerticalBarDecomposition( void );

  /**
   * Get the top and bottom ordinates of each bar in linear coordinates.
   */
  virtual void getBarOrdinates(double bottom[], double top[]);

  /**
   * Get the left and right abscissas of each bar in linear coordinates.
   */
  virtual void getBarAbscissas(double left[], double right[]);

protected:

};

}

#endif /* _VERTICAL_BAR_DECOMPOSITION_HXX_ */
