/*------------------------------------------------------------------------*/
/* file: SegsMarkDrawerJavaMapper.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SegsMarkDrawer          */
/*------------------------------------------------------------------------*/

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
/*--------------------------------------------------------------------------*/
void SegsMarkDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
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

}
