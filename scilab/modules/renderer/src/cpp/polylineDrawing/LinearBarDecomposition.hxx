/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Decompose polyline to get data for bar drawing in linear mode
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

