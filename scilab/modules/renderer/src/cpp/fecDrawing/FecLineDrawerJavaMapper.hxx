/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by FecLineDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "FecLineDrawerGL.hxx"

namespace sciGraphics
{

class FecLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  FecLineDrawerJavaMapper(void);

  virtual ~FecLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for fec line line drawer
  virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_fecDrawing::FecLineDrawerGL * m_pJavaObject;


};

}

#endif /* _FEC_LINE_DRAWER_JAVA_MAPPER_HXX_ */

