/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver independant routine to draw a  
 * legend object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_LEGEND_HXX_
#define _CONCRETE_DRAWABLE_LEGEND_HXX_

#include <string>

#include "DrawableLegend.h"
#include "../subwinDrawing/Camera.h"

namespace sciGraphics
{

class ConcreteDrawableLegend : public DrawableLegend
{
public:

  ConcreteDrawableLegend(sciPointObj * pLegend);

  virtual ~ConcreteDrawableLegend(void);

protected:

  /*-----------------------------------------------*/
  /**
   * Actually draw the legend entity
   */
  virtual void drawLegend(void);

  /**
   * Update the legend entity
   */
  virtual int updateLegend(void);
  /**
   * Show the legend entity
   */
  virtual void showLegend(void);

  /**
   * Build text object displaying names
   */
  void setTextParameters(void);

  /**
   * Create the polylines to use
   */
  void createLines(void);
  /**
   * Create the box
   */
  void createBox(void);

  /**
   * Set polylines properties
   */
  void setLinesParameters(void);

  /**
   * Set legend bounding box properties
   */
  void setBoxParameters(void);

  /**
   * Destroy the polylines
   */
  void destroyLines(void);

  /**
   * Destroy text
   */
  void destroyText(void);

  /**
   * Destroy box
   */
  void destroyBox(void);

  /**
   * Return the number of legends
   */
  int getNbLegend(void) { return pLEGEND_FEATURE(m_pDrawed)->nblegends;}

  /**
   * Get the size in pixels of the text
   */
  void getTextPixelSize(double * width, double * height);

  /**
   * Compute the rectangles in 3d in which the lines will be drawn and also the 3D bounding box rectangle
   */
  void getBoxes(const std::string & opt, double upperLeftCorner[3], double lowerLeftCorner[3],
		double lowerRightCorner[3], double upperRightCorner[3],
		double upperBoxLeftCorner[3], double lowerBoxLeftCorner[3],
		double lowerBoxRightCorner[3], double upperBoxRightCorner[3]);
  /**
   * Place lines just inside the line box
   */
  void placeLines(const double upperLeftCorner[3], const double lowerLeftCorner[3],
                  const double lowerRightCorner[3], const double upperRightCorner[3]);

  /**
   * Place the text just right the text box
   */
  void placeText(const double upperLeftCorner[3], const double lowerLeftCorner[3],
                 const double lowerRightCorner[3], const double upperRightCorner[3]);

 /**
   * Place the legend bounding box
   */
  void placeBox(const double upperLeftCorner[3], const double lowerLeftCorner[3],
                 const double lowerRightCorner[3], const double upperRightCorner[3]);

	/**
	 * @return Camera of the parent subwin.
	 */
	Camera * getCamera(void);



  /*-----------------------------------------------*/
  /** text object drawing the strings */
  sciPointObj * m_pNames;

  /** polylines used to draw the lines in front of names */
  sciPointObj ** m_aLines;
  /*-----------------------------------------------*/
  /** rectangle used to draw the box */
  sciPointObj * m_aBox;
  /*-----------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_LEGEND_HXX_ */

