/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSegs.hxx                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms of drawable segs                                     */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_SEGS_HXX_ 
#define _CONCRETE_DRAWABLE_SEGS_HXX_

#include <list>
#include "DrawableSegs.h"
#include "DrawSegsStrategy.hxx"
#include "DecomposeSegsStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableSegs : public DrawableSegs
{
public:

  ConcreteDrawableSegs(sciPointObj * pSegs);

  ~ConcreteDrawableSegs(void);

  /**
   * Compute the postions of the arraows to display
   */
  virtual void getSegsPos(double startXcoords[], double endXCoords[],
                          double startYCoords[], double endYCoords[],
                          double startZCoords[], double endZcoords[]);

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

  virtual void setDecomposeStrategy(DecomposeSegsStrategy * decomposer);

  virtual void addDrawingStrategy(DrawSegsStrategy * drawer);

  virtual void removeDrawingStrategies(void);

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the segs
   */
  virtual void drawSegs(void);

  /**
   * Draw the segs from computed positions and colors
   */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment);

  /**
   * Show the segs
   */
  virtual void showSegs(void);
  /*---------------------------------------------------------------------------------*/
  std::list<DrawSegsStrategy *> m_oDrawers;
  DecomposeSegsStrategy * m_pDecomposer;
  /*---------------------------------------------------------------------------------*/
  
};

}

#endif /* _CONCRETE_DRAWABLE_SEGS_HXX_ */
