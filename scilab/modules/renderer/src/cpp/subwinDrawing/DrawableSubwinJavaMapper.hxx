/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by DrawableSubwin
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_
#define _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "DrawableSubwinGL.hxx"

namespace sciGraphics
{

/**
 * Contains mapping of java method used by DrawableSubwin 
 */
class DrawableSubwinJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  DrawableSubwinJavaMapper(void);

  virtual ~DrawableSubwinJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for subwins
  virtual void setSubwinParameters(int subwinIndex, bool is2d);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::DrawableSubwinGL * m_pJavaObject;

};

}

#endif /* _DRAWABLE_SUBWIN_JAVA_MAPPER_HXX_ */

