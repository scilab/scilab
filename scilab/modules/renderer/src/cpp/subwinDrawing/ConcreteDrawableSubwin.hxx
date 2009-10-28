/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing drivers independent routines for a subwin object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_SUBWIN_HXX_
#define _CONCRETE_DRAWABLE_SUBWIN_HXX_

#include <list>

#include "DrawableSubwin.h"
#include "ComputeBoundsStrategy.hxx"
#include "DrawAxesBoxStrategy.hxx"
#include "TicksDrawer.hxx"
#include "../labelDrawing/LabelPositioner.hxx"


namespace sciGraphics
{

class ConcreteDrawableSubwin : public DrawableSubwin
{
public:

  ConcreteDrawableSubwin(sciPointObj * pObj);

  virtual ~ConcreteDrawableSubwin(void);

  void setXBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setYBoundsStrategy(ComputeBoundsStrategy * strategy);
  void setZBoundsStrategy(ComputeBoundsStrategy * strategy);

  void setXTicksDrawer(TicksDrawer * xTicksDrawer);
  void setYTicksDrawer(TicksDrawer * yTicksDrawer);
  void setZTicksDrawer(TicksDrawer * zTicksDrawer);

  void addAxesBoxDrawer(DrawAxesBoxStrategy * strategy);
  void removeAxesBoxDrawers(void);

  /**
   * For non linear scaling (not supported by OpenGL) we need to modify points.
   * For direction, use direction Scale.
   */
  virtual void pointScale(double xCoord, double yCoord, double zCoord,
                          double * xScaled, double * yScaled, double * zScaled);


  /**
   * Inverse function of pointScale.
   */
  virtual void inversePointScale(double xScaled, double yScaled, double zScaled,
                                 double * xCoord, double * yCoord, double * zCoord);

  /**
   * Apply scale on a vector.
   */
  virtual void pointScale(double vectorX[], double vectorY[], double vectorZ[], int vectorLength);

  /**
   * Apply non linear scaling on direction vector (opposite as point vector).
   * Since scaling is non linear, we need to have a starting point of the vector.
   */
  virtual void directionScale(double xCoord, double yCoord, double zCoord,
                              double startingPointX, double startingPointY, double startingPointZ,
                              double * xScaled, double * yScaled, double * zScaled);

  /**
   * Apply distance scale on a vector.
   */
  virtual void directionScale(double vectorX[], double vectorY[], double vectorZ[],
                              double startingPointsX[], double startingPointsY[],
                              double startingPointsZ[], double vectorLength);

  /**
   * Compute real data bounds from the user ones.
   * ie fills Frect.
   */
  virtual void computeRealDataBounds(void);

  /**
   * Just update scale and size if needed
   */
  virtual void updateScale(void);

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbXTicks(void);

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  virtual void getXTicksPos(double ticksPositions[], char ** ticksLabels);

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbYTicks(void);

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  virtual void getYTicksPos(double ticksPositions[], char ** ticksLabels);

  /**
   * Get the initial number of ticks (with no reduction) that will be drawn.
   */
  virtual int getNbZTicks(void);

  /**
   * Get the initial ticks positions and labels (with no reduction) that will be drawn.
   * To be used to know the positions and labels from outside the rendering process
   */
  void getZTicksPos(double ticksPositions[], char ** ticksLabels);

  /**
   * Compute bounds of X axis and direction of ticks
   */
  virtual bool getXAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]);

  /**
   * Compute bounds of Y axis and direction of ticks
   */
  virtual bool getYAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]);

  /**
   * Compute bounds of Z axis and direction of ticks
   */
  virtual bool getZAxisPosition(double axisStart[3], double axisEnd[3], double ticksDirection[3]);

  /**
   * Specify that a new text has been added under this object.
   */
  virtual void addTextToDraw(sciPointObj * text);

  /**
   * Specify that a text object has been destoyed and should be removed from
   * the list of drawing texts.
   */
  virtual void removeTextToDraw(sciPointObj * text);

  /**
   * Redefine display children in order to draw the stored text after the other objects.
   * Actually, text objects use OpenGL blending for their transparency.
   * Consequentely they must be drawn after all the primitives which are behind them
   * in order to make the transparency work. To perform that, we draw the text objects
   * after the others (and consequently after the ones behind them). The previous
   * method is not enough since the text objects themselves are not sorted from each other.
   * We then sort them from back to front. Luckily all texts objects are flat and drawn
   * vertically, so each of their point is as far from the viewpoint. It's then easy to compute
   * their depth relative to viewpoint and sort them accordingly.
   * For more informations see: http://www.opengl.org/resources/faq/technical/transparency.htm.
   */
  virtual void displayChildren(void);

  /**
   * To be called when a text object change.
   * Next display will sort the text objects.
   */
  virtual void textChanged(void);


protected:

  /*---------------------------------------------------------------------*/

  /**
   * Draw the surrounding cube of the subwin object.
   */
  virtual void drawBox(void);

  /**
   * Draw the ticks of the subwin object.
   */
  virtual void drawTicks(void);

  /**
   * Draw the surrounding cube of the subwin object from alraedy computed data.
   */
  virtual void showBox(void);

  /**
   * Draw the ticks of the subwin from alraedy computed data.
   */
  virtual void showTicks(void);

  /**
   * Draw the labels after the ticks.
   */
  virtual void displayLabels(void);

  /**
   * Display all the stored text objects
   */
  void displayTexts(void);

  /**
   * Sort the text objects from back to front relatively to view point.
   */
  void sortDisplayedTexts(void);

  /**
   * Set the distance to use between axis and labels.
   * May vary depending on the ticks drawing, the two should not
   * conceal each other.
   */
  virtual void setLabelsDistanceToAxis(double xLabelDist, double yLabelDist,
                                       double zLabelDist, double titleDist);
  /*---------------------------------------------------------------------*/
  ComputeBoundsStrategy * m_pXBoundsStrategy;
  ComputeBoundsStrategy * m_pYBoundsStrategy;
  ComputeBoundsStrategy * m_pZBoundsStrategy;

  TicksDrawer * m_pXTicksDrawer;
  TicksDrawer * m_pYTicksDrawer;
  TicksDrawer * m_pZTicksDrawer;

  std::list<DrawAxesBoxStrategy *> m_oAxesBoxDrawers;

  /** list of text objects to draw at the end, ordered from back to front */
	std::list<sciPointObj *> m_oDisplayedTexts;

  /** Specify wether some texts objects has been added or removed since last update */
  bool m_bTextListChanged;
  /*---------------------------------------------------------------------*/

private:

  /**
   * Compute concealed corner
   * @return index of the coner which is concealed
   */
  int computeConcealedCornerIndex(void);

	/**
   * Compute the distance of a text object to the view point.
   * @param cam camera sed to get the distance.
   */
	static double getEyeDistance(Camera * cam, sciPointObj * pText);

	/**
	 * Compare the viewing depth of two text object.
	 * This functions returns true if pText1 is deeper than pText2.
	 * This function is used to sort texts within the text list.
	 */
	static bool getTextOrder(sciPointObj * pText1, sciPointObj * pText2);

  
};

}

#endif /* _CONCRETE_DRAWABLE_SUBWIN_HXX_ */

