/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _PLACE_TICKS_STRATGY_HXX_
#define _PLACE_TICKS_STRATGY_HXX_

#include "DrawableSubwin.h"

namespace sciGraphics
{

/**
 * Class converting ticks abscissas into pixel positions
 */
class PlaceTicksStrategy
{
public:

  PlaceTicksStrategy(DrawableObject * subwin);

  virtual ~PlaceTicksStrategy(void);

  /**
   * Compute the two edges of axis segment in pixel coordinates.
   */
  virtual void computeAxisBounds(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3]);

  /**
   * Compute direction to be used by ticks in pixel coordinates
   */
  virtual void computeTicksDir(double ticksDir[3]);

  /**
   * Convert ticks abscissas along axis into positions in pixel coordinates.
   * Also remove out of bounds legends and ticks.
   * @param nbTicksIn number of ticks position in ticksAbscissas
   * @param[out] nbTicksOut number of ticks in final ticks positions
   */
  virtual void computeTicksLocation(const double ticksAbscissas[],
                                    double ticksXPos[],
                                    double ticksYPos[],
                                    double ticksZPos[],
                                    char * labels[],
                                    char * labelsExponents[],
                                    int nbTicksIn,
                                    int * nbTicksOut);


  /**
   * Compute the start points to use for grid drawing in pixel coordinates
   */
  virtual void computeGridStartPoints(const double ticksAbscissas[],
                                      const double subticksAbscissas[],
                                      double startXPos[],
                                      double startYPos[],
                                      double startZPos[],
                                      int nbTicksIn,
                                      int nbSubticksin,
                                      int * nbTicksOut) ;
  

  /**
   * Compute the middle points to use for grid drawing in pixel coordinates
   */
  virtual void computeGridMiddlePoints(const double ticksAbscissas[],
                                        const double subticksAbscissas[],
                                        double middleXPos[],
                                        double middleYPos[],
                                        double middleZPos[],
                                        int nbTicksIn,
                                        int nbSubticksin,
                                        int * nbTicksOut);

  /**
   * Compute the middle points to use for grid drawing in pixel coordinates
   */
  virtual void computeGridEndPoints(const double ticksAbscissas[],
                                    const double subticksAbscissas[],
                                    double endXPos[],
                                    double endYPos[],
                                    double endZPos[],
                                    int nbTicksIn,
                                    int nbSubticksin,
                                    int * nbTicksOut);

  /**
   * Specify wether the axis segment should be draw or not
   * true by default
   */
  virtual bool isAxisSegmentDrawn(void) {return true;}


protected:

  /*----------------------------------------------------------------------*/
  /**
   * Compute the two edges of axis segment in 3D coordinates
   */
  virtual void computeAxisBounds3D(const double ticksAbscissas[], int nbTicks, double axisStart[3], double axisEnd[3]) = 0;

  /**
   * Compute direction to be used by ticks in 3D coordinates
   */
  virtual void computeTicksDir3D(double ticksDir[3]) = 0;


  /**
   * Convert ticks abscissas along axis into positions in 3D coordinates
   * Also remove out of bounds legends and ticks.
   * @param nbTicksIn number of ticks position in ticksAbscissas
   * @param[out] nbTicksOut number of ticks in final ticks positions
   */
  virtual void computeTicksLocation3D(const double ticksAbscissas[],
                                      double ticksXPos[],
                                      double ticksYPos[],
                                      double ticksZPos[],
                                      char * labels[],
                                      char * labelsExponents[],
                                      int nbTicksIn,
                                      int * nbTicksOut) = 0;

  /**
   * Compute the start points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridStartPoints3D(const double ticksAbscissas[],
                                        const double subticksAbscissas[],
                                        double startXPos[],
                                        double startYPos[],
                                        double startZPos[],
                                        int nbTicksIn,
                                        int nbSubicksin,
                                        int * nbTicksOut) {};

  /**
   * Compute the middle points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridMiddlePoints3D(const double ticksAbscissas[],
                                         const double subticksAbscissas[],
                                         double middleXPos[],
                                         double middleYPos[],
                                         double middleZPos[],
                                         int nbTicksIn,
                                         int nbSubicksin,
                                         int * nbTicksOut) {};

  /**
   * Compute the middle points to use for grid drawing in 3D coordinates
   * To be redifined by ticks which also draw grid
   */
  virtual void computeGridEndPoints3D(const double ticksAbscissas[],
                                      const double subticksAbscissas[],
                                      double endXPos[],
                                      double endYPos[],
                                      double endZPos[],
                                      int nbTicksIn,
                                      int nbSubicksin,
                                      int * nbTicksOut) {};

  /**
   * Knowing one of the bounds along X axis, return the other
   */
  double getOtherXBound(double xBound);

  /**
   * Knowing one of the bounds along Y axis, return the other
   */
  double getOtherYBound(double yBound);

  /**
   * Knowing one of the bounds along Z axis, return the other
   */
  double getOtherZBound(double zBound);

  /**
   * Compute the X coordinate of the Y axis segment
   * Select the one wich is in front of the camera
   * @param zCoordinate zCoordinate Z coordinate of the Y axis segment
   * @return X coordinate of the segment to draw
   */
  double findFrontXCoordinate(double zCoordinate);

  /**
   * Compute the X coordinate of the Y axis segment
   * Select the one wich is in back of the camera
   * @param zCoordinate zCoordinate Z coordinate of the Y axis segment
   * @return X coordinate of the segment to draw
   */
  double findBackXCoordinate(double zCoordinate);

  /**
   * Find the height of the bottom facet
   * @return Z coordinate of the segment to draw
   */
  double findLowerZCoordinate(void);

  /**
   * Find the height of the upper facet
   * @return Z coordinate of the segment to draw
   */
  double findUpperZCoordinate(void);

  /**
   * Compute the Y coordinate of the X axis segment
   * Select the one wich is in front of the camera
   * @param zCoordinate Z coordinate of the X axis segment already computed by findZCoordinate
   * @return Y coordinate of the segment to draw
   */
  double findFrontYCoordinate(double zCoordinate);

  /**
   * Compute the Y coordinate of the X axis segment
   * Select the one wich is in back of the camera
   * @param zCoordinate Z coordinate of the X axis segment already computed by findZCoordinate
   * @return Y coordinate of the segment to draw
   */
  double findBackYCoordinate(double zCoordinate);

  /**
   * Compute the Y coordinate of the Z axis segment
   * @return Y coordinate of the segment to draw
   */
  double findLeftMostYCoordinate(void);

  /**
   * Find the X coordinate of the Z axis segment.
   * Z axis segment is always displayed on the left of the box.
   * @return X coordinate of the segment to draw
   */
  double findLeftMostXCoordinate(void);

  /**
  * Chack if the ticks direction is not too close to the axis segment. That would lead to a bad display.
  * @param ticksDirection direction of ticks
  * @param axisSegmentStart one edge of the axis segment
  * @param axisSegmentEnd the other edge
  * @return true if ticks direction is OK, false otherwise
  */
  bool checkTicksDirection(const double ticksDirection[3],
                           const double axisSegmentStart[3],
                           const double axisSegmentEnd[3]);

  /**
   * Use Z direction as ticks direction
   * @param zCoordinate coordinate of the axis
   */
  void getZDirection(double ticksDirection[3], double zCoordinate);

  /**
   * Convert user coordinates to pixel coordinates
   */
  void getPixelCoordinates(const double userCoords[3], double pixCoords[3]);

  /**
   * Update xmin, xmax, ymin, ymax, ...
   */
  void updateDataBounds(void);

  /**
   * Set a new ticks position with computed coordinates
   * also permuts labels
   */
  void setPosition(double ticksXPos[], double ticksYPos[], double ticksZPos[],
                   double xCoordinate, double yCoordinate, double zCoordinate,
                   char * labels[], char * labelsExponents[], int newTicksIndex, int oldTicksIndex);

  double getXmin(void) { return m_dXmin; }
  double getXmax(void) { return m_dXmax; }
  double getYmin(void) { return m_dYmin; }
  double getYmax(void) { return m_dYmax; }
  double getZmin(void) { return m_dZmin; }
  double getZmax(void) { return m_dZmax; }

  /*----------------------------------------------------------------------*/
  DrawableObject * m_pDrawer;
  double m_dXmin;
  double m_dXmax;
  double m_dYmin;
  double m_dYmax;
  double m_dZmin;
  double m_dZmax;
  /*----------------------------------------------------------------------*/

};

}

#endif /* _PLACE_TICKS_STRATGY_HXX_ */
