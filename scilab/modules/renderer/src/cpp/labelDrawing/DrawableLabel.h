/*------------------------------------------------------------------------*/
/* file: DrawableLabel.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a label object         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LABEL_H_
#define _DRAWABLE_LABEL_H_


#include "../DrawableObject.h"
#include "LabelPositioner.hxx"

namespace sciGraphics
{

class DrawableLabel : public DrawableObject
{

public:

  DrawableLabel( sciPointObj * pObj );

  virtual ~DrawableLabel(void);

  /**
   * Set the distance to use between the axis and label
   */
  void setDistanceToAxis(double dist);

  /**
   * Return the real type of implementation object
   * Here it is just getting a pointer on the object used to place label
   * The label is drawn is drawn using its text object
   */
  LabelPositioner * getLabelPositioner( void ) ;

protected:

  /*------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

  /**
   * Automatically set label position and orientation
   */
  void setLabelLocation(void);

  /**
   * Automatically set label position
   */
  void setLabelPosition(void);

  /**
   * Automatically set label orientation
   */
  void setLabelOrientation(void);
  /*------------------------------------------------------------------*/



} ;

}

#endif /* _DRAWABLE_LABEL_H_  */
