/*------------------------------------------------------------------------*/
/* file: HorizontalBarDecomposition.hxx                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline to get data for horizontal bar drawing       */
/*------------------------------------------------------------------------*/

#ifndef _HORIZONTAL_BAR_DECOMPOSITION_HXX_
#define _HORIZONTAL_BAR_DECOMPOSITION_HXX_

#include "LinearBarDecomposition.hxx"
#include "BarDecomposition.hxx"

namespace sciGraphics
{

class HorizontalBarDecomposition : public LinearBarDecomposition
{
public:

  HorizontalBarDecomposition( BarDecomposition * decomposition );

  virtual ~HorizontalBarDecomposition( void );

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

#endif /* _HORIZONTAL_BAR_DECOMPOSITION_HXX_ */
