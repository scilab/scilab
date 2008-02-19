/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class positioning Z label using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef _Z_LABEL_POSITIONER_JOGL_HXX_
#define _Z_LABEL_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"

namespace sciGraphics
{

class ZLabelPositionerJoGL : public LabelPositionerJoGL
{
public:

  ZLabelPositionerJoGL(DrawableLabel * label);

  virtual ~ZLabelPositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

};

}

#endif /* _Z_LABEL_POSITIONER_JOGL_HXX_ */
