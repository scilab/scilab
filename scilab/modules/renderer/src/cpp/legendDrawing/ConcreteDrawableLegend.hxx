/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableLegend.hxx                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independant routine to draw a       */
/*        legend object                                                   */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_LEGEND_HXX_
#define _CONCRETE_DRAWABLE_LEGEND_HXX_

#include "DrawableLegend.h"

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
   * Set polylines properties
   */
  void setLinesParameters(void);


  /**
   * Compute a matrix of strings from the "toto@titi@tata" one.
   */
  void computeTextMatrix(char ** textMat);

  /**
   * Destroy the polylines
   */
  void destroyLines(void);

  /**
   * Destroy text
   */
  void destroyText(void);

  /**
   * Return the numbe rof legends
   */
  int getNbLegend(void) { return pLEGEND_FEATURE(m_pDrawed)->nblegends;}

  /**
   * Get the size in pixels of the text
   */
  void getTextPixelSize(double * width, double * height);

  /**
   * Compute the rectangle in 3d in which the lines will be drawn
   */
  void getLineBox(double upperLeftCorner[3], double lowerLeftCorner[3],
                  double lowerRightCorner[3], double upperRightCorner[3]);

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

  /*-----------------------------------------------*/
  /** text object drawing the strings */
  sciPointObj * m_pNames;

  /** polylines used to draw the lines in front of names */
  sciPointObj ** m_aLines;
  /*-----------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_LEGEND_HXX_ */
