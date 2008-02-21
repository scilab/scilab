/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy decomposing only champ objects         
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CHAMP_DECOMPOSER_HXX_ 
#define _CHAMP_DECOMPOSER_HXX_

#include "DecomposeSegsStrategy.hxx"

namespace sciGraphics
{

class ChampDecomposer : public DecomposeSegsStrategy
{
public:

  ChampDecomposer(DrawableSegs * segs);

  virtual ~ChampDecomposer(void);

  /**
  * Compute the postions of the arraows to display
  */
  virtual void getSegsPos(double startXCoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[],
                          double startZCoords[], double endZCoords[]);

  /**
  * Get the number of arrows in the segs or champ object
  */
  virtual int getNbSegment(void);

  /**
  * To know if each segs object has a distinct color
  */
  virtual bool isColored(void);

  /**
  * Get the color of each segement if needed
  */
  virtual void getSegsColors(int colors[]);

protected:


  /**
   * Compute the vector [vx,vy] = [x,y] + [vx,vy];
   */
  void getDefaultChampPos(double startXCoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[]);

  /**
   * Get coordinates for a champ object (not colored)
   */
  void getChampPos(double startXCoords[], double endXCoords[],
                   double startYCoords[], double endYCoords[]);

  /**
   * Get coordinates for a champ1 object (colored).
   */
  void getChamp1Pos(double startXCoords[], double endXCoords[],
                    double startYCoords[], double endYCoords[]);

  /**
   * Get colors for champ object (single color)
   */
  void getChampColors(int colors[]);

  /**
   * Get colors for champ1 object (color depending on vectors length).
   */
  void getChamp1Colors(int colors[]);

  /**
   * Get the length of the longest vector
   */
  double getMaxLength(void);

private:

  /**
   * Find the maximum length usable by segments
   */
  double computeMaxUsableLength(void);

};

}

#endif /* _CHAMP_DECOMPOSER_HXX_ */

