/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by BackTrihedronJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _BACK_TRIHEDRON_DRAWER_JAVA_MAPPER_HXX_
#define _BACK_TRIHEDRON_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "BackTrihedronDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by BackTrihedronDrawerJoGL 
 */
class BackTrihedronDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  BackTrihedronDrawerJavaMapper(void);

  virtual ~BackTrihedronDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for back trihedron
  virtual void setBoxParameters(int hiddenAxisColor, float thickness);
  virtual void drawBox(double xMin, double xMax, double yMin,
                       double yMax, double zMin, double zMax,
                       int concealedCornerIndex);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::BackTrihedronDrawerGL * m_pJavaObject;

};

}

#endif /* _BACK_TRIHEDRON_DRAWER_JAVA_MAPPER_HXX_ */

