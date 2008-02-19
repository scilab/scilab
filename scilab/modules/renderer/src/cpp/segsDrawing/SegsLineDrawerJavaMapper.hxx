/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by SegsLineDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SEGS_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _SEGS_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SegsLineDrawerGL.hxx"

namespace sciGraphics
{

class SegsLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  SegsLineDrawerJavaMapper(void);

  virtual ~SegsLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for segs line drawer
  virtual void setLineParameters(float thickness, int lineStyle);

  virtual void drawSegs(const double startXCoords[], const double endXCoords[],
                        const double startYCoords[], const double endYCoords[],
                        const double startZCoords[], const double endZCoords[],
                        const int colors[], int nbSegments);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_segsDrawing::SegsLineDrawerGL * m_pJavaObject;
  

};

}

#endif /* _SEGS_LINE_DRAWER_JAVA_MAPPER_HXX_ */

