/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by TitlePositionerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TITLE_POSITIONER_JAVA_MAPPER_HXX_
#define _TITLE_POSITIONER_JAVA_MAPPER_HXX_

#include "LabelPositionerJavaMapper.hxx"
#include "TitlePositionerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by TitlePositionerJoGL 
 */
class TitlePositionerJavaMapper : public virtual LabelPositionerJavaMapper
{
public:

  TitlePositionerJavaMapper(void);

  virtual ~TitlePositionerJavaMapper(void);
  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for LabelsPositioner
  virtual void getLabelPosition(double distanceToAxis, double labelPos[3]);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void setLabelPixBoundingBox(int corner1[2], int corner2[2],
                                      int corner3[2], int corner4[2]);
  /*----------------------------------------------------------------------*/
  // specific for TitlePositioner
  virtual void setViewingAreaUpperPos(int xPos, int yPos);
  /*----------------------------------------------------------------------*/


protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_labelDrawing::TitlePositionerGL * m_pJavaObject;

};

}

#endif /* _TITLE_POSITIONER_JAVA_MAPPER_HXX_ */

