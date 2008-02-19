/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Decompose polyline to get data for vertical bar drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

