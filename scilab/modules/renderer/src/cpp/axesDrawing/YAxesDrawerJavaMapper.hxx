/*------------------------------------------------------------------------*/
/* file: YAxesDrawerJavaMapper.hxx                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by YAxesDrawerJoGL         */
/*------------------------------------------------------------------------*/

#ifndef _Y_AXES_DRAWER_JAVA_MAPPER_HXX_
#define _Y_AXES_DRAWER_JAVA_MAPPER_HXX_

#include "../subwinDrawing/TicksDrawerJavaMapper.hxx"
#include "YAxesDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by XTicksDrawerJoGL 
 */
class YAxesDrawerJavaMapper : public virtual TicksDrawerJavaMapper
{
public:

  YAxesDrawerJavaMapper(void);

  virtual ~YAxesDrawerJavaMapper(void);
  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // Inherited from TicksDrawer
  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[],
                           int nbTicks, const double subticksPositions[], int nbSubticks);

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks);

  virtual double drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                           int nbTicks, const double subticksPositions[], int nbSubticks);

  virtual bool checkTicks(const double ticksPositions[], char * ticksLabels[],
                          char * labelsExponents[], int nbTicks);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                  int fontType, double fontSize, int fontColor);
  /*----------------------------------------------------------------------*/
  // specific for YAxesDrawer
  virtual void setConstantCoordinates(double xCoordinate, double zCoordinate);

  virtual void setTicksDirection(char dir);

  virtual void setAxisLineDrawing(bool drawAxisLine);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_axesDrawing::YAxesDrawerGL * m_pJavaObject;

};

}

#endif /* _Y_AXES_DRAWER_JAVA_MAPPER_HXX_ */
