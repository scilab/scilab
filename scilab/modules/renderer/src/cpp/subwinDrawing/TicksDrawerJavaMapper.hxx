/*------------------------------------------------------------------------*/
/* file: TicksDrawerJavaMapper.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by TicksDrawer             */
/*------------------------------------------------------------------------*/

#ifndef _TICKS_DRAWER_JAVA_MAPPER_HXX_
#define _TICKS_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"


namespace sciGraphics
{

/**
* Contains mapping of java method used by HalfBoxDrawerJoGL 
*/
class TicksDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  TicksDrawerJavaMapper(void) : DrawableObjectJavaMapper() {}

  virtual ~TicksDrawerJavaMapper(void) {}

  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual void drawTicks(const double ticksPositions[], char * ticksLabels[],
                         int nbTicks, const double subticksPositions[], int nbSubticks) = 0;

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks) = 0;

  virtual void drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                         int nbTicks, const double subticksPositions[], int nbSubticks) = 0;

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[],
                          char * labelsExponents[], int nbTicks) = 0;

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax) = 0;

  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor) = 0;
  /*----------------------------------------------------------------------*/

protected:


};

}

#endif /* _TICKS_DRAWER_JAVA_MAPPER_HXX_ */
