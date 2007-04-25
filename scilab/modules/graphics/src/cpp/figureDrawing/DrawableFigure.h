/*------------------------------------------------------------------------*/
/* file: DrawableFigure.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a figure object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_H_
#define _DRAWABLE_FIGURE_H_

#include "../DrawableObject.h"
#include "DrawableFigureImp.h"

namespace sciGraphics
{

class DrawableFigure : public DrawableObject
{

public:

  DrawableFigure( sciPointObj * pObj ) ;

  virtual ~DrawableFigure( void ) ;

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

  /*--------------------------------------------------------------------------------------*/
  // Driver Independant Algorithms
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) = 0 ;

  /*--------------------------------------------------------------------------------------*/
  // Driver dependant algorithms
  /**
   * Initialize the context for drawing
   */
  void initializeDrawing( void ) ;

  /**
   * Close drawing session and display image on the screen
   */
  void endDrawing( void ) ;

  /**
   * Display the info message of the window
   */
  void updateInfoMessage( void ) ;

  /**
   * Set the implementation objects for the previous algorithmz
   */
  void setDrawableImp( DrawableFigureImp * imp ) { m_pImp = imp ; }

  /*--------------------------------------------------------------------------------------*/ 

protected:

  DrawableFigureImp * m_pImp ; /**< bridge for driver dependant algorithm */

} ;

}

#endif /* _DRAWABLE_FIGURE_H_  */
