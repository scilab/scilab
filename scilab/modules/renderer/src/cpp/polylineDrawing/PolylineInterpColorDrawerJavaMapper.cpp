/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by PolylineLineDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "PolylineInterpColorDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJavaMapper::PolylineInterpColorDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineInterpColorDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJavaMapper::~PolylineInterpColorDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::drawPolyline(const double xCoords[], const double yCoords[],
                                                       const double zCoords[], const int colors[], int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)xCoords, nbCoords,
                              (double *)yCoords, nbCoords,
                              (double *)zCoords, nbCoords,
                              (int *)colors,  nbCoords);
}
/*---------------------------------------------------------------------------------*/
}
