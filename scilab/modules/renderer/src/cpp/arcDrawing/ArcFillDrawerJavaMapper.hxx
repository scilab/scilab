/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by ArcFillDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _ARC_FILL_DRAWER_JAVA_MAPPER_HXX_
#define _ARC_FILL_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "ArcFillDrawerGL.hxx"

namespace sciGraphics
{

class ArcFillDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  ArcFillDrawerJavaMapper(void);

  virtual ~ArcFillDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle fill drawer
  virtual void setBackColor(int color);

  virtual void setUseNurbs(bool useNurbs);

  virtual void drawArc( double centerX       , double centerY       , double centerZ       ,
                        double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
                        double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
                        double startAngle    , double endAngle );

  virtual void redrawArc(void);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_arcDrawing::ArcFillDrawerGL * m_pJavaObject;


};

}

#endif /* _ARC_FILL_DRAWER_JAVA_MAPPER_HXX_ */

