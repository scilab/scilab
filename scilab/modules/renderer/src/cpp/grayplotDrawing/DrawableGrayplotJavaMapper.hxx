/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by DrawableGrayplot
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_GRAYPLOT_JAVA_MAPPER_HXX_
#define _DRAWABLE_GRAYPLOT_JAVA_MAPPER_HXX_

#include "DrawableClippedObjectJavaMapper.hxx"
#include "DrawableGrayplotGL.hxx"

namespace sciGraphics
{

class DrawableGrayplotJavaMapper : public virtual DrawableClippedObjectJavaMapper
{
public:

  DrawableGrayplotJavaMapper(void) ;

  virtual ~DrawableGrayplotJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);

  virtual void translate(const double translation[3]);

  virtual void endTranslate(void);
  /*----------------------------------------------------------------------*/
  // Inherited From DrawableClippedObjectJavaMapper
  virtual void clipX(double xMin, double xMax);
  virtual void clipY(double yMin, double yMax);
  virtual void clipZ(double zMin, double zMax);

  virtual void unClip(void);
  /*----------------------------------------------------------------------*/
  // Specific to grayplot
  virtual void drawGrayplot(const double xGrid[], int nbCol,
                            const double yGrid[], int nbRow,
                            double zCoord, const int colors[], int nbColors);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_grayplotDrawing::DrawableGrayplotGL * m_pJavaObject;

};

}

#endif /* _DRAWABLE_GRAYPLOT_JAVA_MAPPER_HXX_ */
