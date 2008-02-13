/*------------------------------------------------------------------------*/
/* file: LabelPositionerJoGL.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in positioning labels using JoGL              */
/*------------------------------------------------------------------------*/

#include "LabelPositionerJoGL.hxx"
#include "GetJavaProperty.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
LabelPositionerJoGL::LabelPositionerJoGL(DrawableLabel * label)
  : LabelPositioner(), DrawableObjectJoGL(label)
{
  
}
/*------------------------------------------------------------------------------------------*/
LabelPositionerJoGL::~LabelPositionerJoGL(void)
{
  
}
/*------------------------------------------------------------------------------------------*/
void LabelPositionerJoGL::getAutoPosition(double pos[3])
{
  initializeDrawing();
  sciPointObj * pLabel = getDrawer()->getDrawedObject();
  double dataBounds[6];
  sciGetRealDataBounds(sciGetParent(pLabel), dataBounds);

  getLabelPositionerJavaMapper()->setAxesBounds(dataBounds[0], dataBounds[1], dataBounds[2],
                                                dataBounds[3], dataBounds[4], dataBounds[5]);

  int corner1[2];
  int corner2[2];
  int corner3[2];
  int corner4[2];
  sciGetJavaPixelBoundingBox(pLABEL_FEATURE(pLabel)->text, corner1, corner2, corner3, corner4);

  getLabelPositionerJavaMapper()->setLabelPixBoundingBox(corner1, corner2, corner3, corner4);

  getLabelPositionerJavaMapper()->getLabelPosition(m_dDistanceToAxis, pos);

  // convert the data
  m_pDrawer->inversePointScale(pos[0], pos[1], pos[2],
                               &(pos[0]), &(pos[1]), &(pos[2]));

  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
LabelPositionerJavaMapper * LabelPositionerJoGL::getLabelPositionerJavaMapper(void)
{
  return dynamic_cast<LabelPositionerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
