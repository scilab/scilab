/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by ZLabelPositionerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ZLabelPositionerJavaMapper.hxx"


extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
ZLabelPositionerJavaMapper::ZLabelPositionerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_labelDrawing::ZLabelPositionerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
ZLabelPositionerJavaMapper::~ZLabelPositionerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::getLabelPosition(double distanceToAxis, double labelPos[3])
{
  // array of size 3
  double * labelPosJava = m_pJavaObject->getLabelPosition(distanceToAxis);
  labelPos[0] = labelPosJava[0];
  labelPos[1] = labelPosJava[1];
  labelPos[2] = labelPosJava[2];

  delete[] labelPosJava;

}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::setAxesBounds(double xMin, double xMax,
                                               double yMin, double yMax,
                                               double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void ZLabelPositionerJavaMapper::setLabelPixBoundingBox(int corner1[2], int corner2[2],
                                                        int corner3[2], int corner4[2])
{
  long allInOne[8];
  allInOne[0] = corner1[0];
  allInOne[1] = corner1[1];
  allInOne[2] = corner2[0];
  allInOne[3] = corner2[1];
  allInOne[4] = corner3[0];
  allInOne[5] = corner3[1];
  allInOne[6] = corner4[0];
  allInOne[7] = corner4[1];

  m_pJavaObject->setLabelPixBoundingBox(allInOne, 8);
}
/*--------------------------------------------------------------------------*/
}
