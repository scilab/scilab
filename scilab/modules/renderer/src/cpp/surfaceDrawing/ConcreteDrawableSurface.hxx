/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSurface.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#ifndef _CONCRETE_DRAWABLE_SURFACE_HXX_
#define _CONCRETE_DRAWABLE_SURFACE_HXX_

#include <list>

#include "DrawableSurface.h"
#include "DrawSurfaceStrategy.hxx"


namespace sciGraphics
{

class ConcreteDrawableSurface : public DrawableSurface
{
public:
  ConcreteDrawableSurface(sciPointObj * pSurface);

  ~ConcreteDrawableSurface(void);

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy(DrawSurfaceStrategy * strategy);

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies(void);

protected:
  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the surface on the screen
   */
  virtual void drawSurface(void);

  /**
   * Actually show the surface stored data
   */
  virtual void showSurface(void);
  /*---------------------------------------------------------------------------------*/
  std::list<DrawSurfaceStrategy *> m_oDrawingStrategies;
  /*---------------------------------------------------------------------------------*/
};

}

#endif /* _CONCRETE_DRAWABLE_SURFACE_HXX_ */
