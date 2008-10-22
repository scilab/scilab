/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver independent routine for drawing
 * a grayplot object     
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_GRAYPLOT_HXX_
#define _CONCRETE_DRAWABLE_GRAYPLOT_HXX_

#include "DrawableGrayplot.h"
#include "GrayplotDecompositionStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableGrayplot : public DrawableGrayplot
{
public:

  ConcreteDrawableGrayplot(sciPointObj * pObj);

  ~ConcreteDrawableGrayplot(void);


  void setDeccompositionStrategy(GrayplotDecompositionStrategy * strategy);

protected:
  /*----------------------------------------------------------*/
  /**
   * Actually draw the object
   */
  virtual EDisplayStatus drawGrayplot(void);

  /**
   * show the object
   */ 
  virtual void showGrayplot(void);
  /*----------------------------------------------------------*/
  GrayplotDecompositionStrategy * m_pDecomposer;
  /*----------------------------------------------------------*/


};

}

#endif /* _CONCRETE_DRAWABLE_GRAYPLOT_HXX_ */

