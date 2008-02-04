/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotJoGL.cpp                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        grayplot object with JoGL                                       */
/*------------------------------------------------------------------------*/

#include "DrawableGrayplotJoGL.hxx"
#include "DrawableGrayplot.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableGrayplotJoGL::DrawableGrayplotJoGL( DrawableGrayplot * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJoGL::~DrawableGrayplotJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotJoGL::drawGrayplot(const double xGrid[], int nbRow,
                                        const double yGrid[], int nbCol,
                                        double zCoord, const int colors[],
                                        int nbColors)
{
  getGrayplotJavaMapper()->drawGrayplot(xGrid, nbRow, yGrid, nbCol,
                                        zCoord, colors, nbColors);
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplot * DrawableGrayplotJoGL::getGrayplotDrawer( void )
{
  return dynamic_cast<DrawableGrayplot *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableGrayplotJavaMapper * DrawableGrayplotJoGL::getGrayplotJavaMapper(void)
{
  return dynamic_cast<DrawableGrayplotJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
