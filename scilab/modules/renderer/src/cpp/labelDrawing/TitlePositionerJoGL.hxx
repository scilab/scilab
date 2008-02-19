/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class positioning Tile label using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef _TITLE_POSITIONER_JOGL_HXX_
#define _TITLE_POSITIONER_JOGL_HXX_

#include "LabelPositionerJoGL.hxx"
#include "TitlePositionerJavaMapper.hxx"

namespace sciGraphics
{

class TitlePositionerJoGL : public LabelPositionerJoGL
{
public:

  TitlePositionerJoGL(DrawableLabel * label);

  virtual ~TitlePositionerJoGL(void);

protected:

  /**
   * Compute the automatic position of the label
   */
  virtual double getAutoOrientation(void);

  /**
   * Compute the automatic position of the label
   */
  virtual void getAutoPosition(double pos[3]);

  /**
   * Get the object performing mapping with Java class.
   */
  TitlePositionerJavaMapper * getTitlePositionerJavaMapper(void);

};

}

#endif /* _TITLE_POSITIONER_JOGL_HXX_ */
