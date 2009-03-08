/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by SegsMarkDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJavaMapper::SegsMarkDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::SegsMarkDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJavaMapper::~SegsMarkDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::setMarkParameters(int background, int foreground, int markSizeUnit,
                                                      int markSize, int markStyleIndex)
{
  m_pJavaObject->setMarkParameters(background, foreground, markSizeUnit,
                                   markSize, markStyleIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::drawSegs(const double startXCoords[], const double endXCoords[],
                                        const double startYCoords[], const double endYCoords[],
                                        const double startZCoords[], const double endZCoords[],
                                        int nbSegments)
{

  m_pJavaObject->drawSegs((double *)startXCoords, nbSegments,
                          (double *)endXCoords, nbSegments,
                          (double *)startYCoords, nbSegments,
                          (double *)endYCoords, nbSegments,
                          (double *)startZCoords, nbSegments,
                          (double *)endZCoords, nbSegments);

}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::drawSegs(void)
{
  m_pJavaObject->drawSegs();
}
/*---------------------------------------------------------------------------------*/

}
