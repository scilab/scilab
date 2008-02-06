/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableFec.hxx                                          */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver independent routines for drawing    */
/*        a fec object                                                    */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_FEC_HXX_
#define _CONCRETE_DRAWABLE_FEC_HXX_

#include <list>

#include "DrawableFec.h"
#include "DrawFecStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawableFec : public DrawableFec
{
public:

  ConcreteDrawableFec(sciPointObj * pFec);

  virtual ~ConcreteDrawableFec(void);

  /**
   * Add a new strategy for fec drawing
   */
  void addDrawingStrategy(DrawFecStrategy * strategy);

  /**
   * Remove all the currently used stategies for fec drawing
   */
  void removeDrawingStrategies(void);

protected:

  /*-------------------------------------------*/
  /**
   * Actually draw the fec objets
   */
  virtual void drawFec(void);

  /**
   * Draw fec object from precomputed data
   */
  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);

  /**
   * Actually show fec object
   */
  virtual void showFec(void);

  /**
   * Get the number of nodes in the fec object
   */
  int getNbNodes(void);

  /**
   * Get the number of triangles to draw
   */
  int getNbTriangles(void);

  /**
   * Decompose the fec object into nodes positions (xCoords, yCoords).
   * and the triangles indices (firstPoint, secondPoint, thirdPoint).
   */
  void decomposeFec(double xCoords[], double yCoords[],
                    int firstPoints[], int secondPoints[], int thirdPoints[]);
  /*-------------------------------------------*/
  std::list<DrawFecStrategy *> m_oDrawingStrategies;
  /*-------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_FEC_HXX_ */

