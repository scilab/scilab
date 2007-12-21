/*------------------------------------------------------------------------*/
/* file: GridDrawerJavaMapper.hxx                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by GridDrawerJoGL          */
/*------------------------------------------------------------------------*/

#ifndef _GRID_DRAWER_JAVA_MAPPER_HXX_
#define _GRID_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by GridDrawerJoGL 
 */
class GridDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  GridDrawerJavaMapper(void) : DrawableObjectJavaMapper() {}

  virtual ~GridDrawerJavaMapper(void) {}

  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual void drawGrid(const double gridPositions[], int nbPositions) = 0;

  virtual void setGridParameters(int gridColor, float gridThickness) = 0;

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax) = 0;
  /*----------------------------------------------------------------------*/

protected:


};

}

#endif /* _GRID_DRAWER_JAVA_MAPPER_HXX_ */
