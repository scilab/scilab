/*------------------------------------------------------------------------*/
/* file: ConcreteDrawablePolyline.hxx                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_POLYLINE_HXX_
#define _CONCRETE_DRAWABLE_POLYLINE_HXX_

#include "DrawablePolyline.h"
#include <list>
#include "DrawPolylineStrategy.hxx"
#include "DecomposeLineStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawablePolyline : public DrawablePolyline
{
public:

  ConcreteDrawablePolyline(sciPointObj * pObj);

  virtual ~ConcreteDrawablePolyline(void);

  /**
   * Retrieve the vertices of the polyline which will be used to draw it directly.
   * In the polyline sciPointObj, x, y and z vectors does not always stands for the real vertices
   * of the drawn line (specially when staircase mode is on).
   * To allocate the array for retrieving vertices, user shoud use the getDrawnVerticesLength.
   * @param xCoords X coordinates of the drawn vertices
   * @param yCoords Y coordinates of the drawn vertices
   * @param zCoords Z coordinates of the drawn vertices
   */
  virtual void getDrawnVertices(double xCoords[], double yCcoords[], double zCcoords[]);

  /**
   * @return number of vertices which will be computed by the getDrawnVertices routine.
   */
  virtual int getDrawnVerticesLength(void);

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy(DrawPolylineStrategy * strategy);

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies(void);

  /**
   * Add an algorithm for decomposing line.
   */
  void setDecompositionStrategy(DecomposeLineStrategy * strategy);

  /**
   * Remove all algorithms for decomposing lines.
   */
  void removeDecompositionStrategy(void);


protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the arc on the screen
   */
  virtual void drawPolyline(void);

  /**
   * Actually show the arc stored data
   */
  virtual void showPolyline(void);
  /*---------------------------------------------------------------------------------*/
  std::list<DrawPolylineStrategy *> m_oDrawingStrategies;
  DecomposeLineStrategy * m_pDecomposeStrategy;
  /*---------------------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_POLYLINE_HXX_ */