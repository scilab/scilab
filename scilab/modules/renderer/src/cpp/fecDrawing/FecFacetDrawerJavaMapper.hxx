/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by FecFacetDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_
#define _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "FecFacetDrawerGL.hxx"

namespace sciGraphics
{

class FecFacetDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  FecFacetDrawerJavaMapper(void);

  virtual ~FecFacetDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for fec facet line drawer
  virtual void setFacetParameters(double zMin, double zMax, int colMin,
                                  int colMax, int colOutLow, int colOutUp);

  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_fecDrawing::FecFacetDrawerGL * m_pJavaObject;


};

}

#endif /* _FEC_FACET_DRAWER_JAVA_MAPPER_HXX_ */

