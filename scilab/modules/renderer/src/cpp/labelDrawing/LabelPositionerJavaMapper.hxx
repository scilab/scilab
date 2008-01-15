/*------------------------------------------------------------------------*/
/* file: LabelPositionerJavaMapper.hxx                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by LabelPositionerJoGL     */
/*------------------------------------------------------------------------*/

#ifndef _LABEL_POSITIONER_JAVA_MAPPER_HXX_
#define _LABEL_POSITIONER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"


namespace sciGraphics
{

/**
* Contains mapping of java method used by TicksDrawerJoGL 
*/
class LabelPositionerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  LabelPositionerJavaMapper(void) : DrawableObjectJavaMapper() {}

  virtual ~LabelPositionerJavaMapper(void) {}

  /*----------------------------------------------------------------------*/
  // specific for LabelsPositioner
  virtual void getLabelPosition(double distanceToAxis, double labelPos[3]) = 0;

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax) = 0;

  virtual void setLabelPixBoundingBox(int corner1[2], int corner2[2],
                                      int corner3[2], int corner4[2]) = 0;
  /*----------------------------------------------------------------------*/

protected:


};

}

#endif /* _LABEL_POSITIONER_JAVA_MAPPER_HXX_ */
