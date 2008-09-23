/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a label object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_LABEL_H_
#define _DRAWABLE_LABEL_H_


#include "DrawableObject.h"
#include "LabelPositioner.hxx"

namespace sciGraphics
{

class DrawableLabel : public DrawableObject
{

public:

  DrawableLabel( sciPointObj * pObj );

  virtual ~DrawableLabel(void);

  void setLabelPositioner(LabelPositioner * positioner);

  /**
   * Set the distance to use between the axis and label
   */
  void setDistanceToAxis(double dist);

protected:

  /*------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Automatically set label position and orientation
   */
  bool setLabelLocation(void);

  /**
   * Automatically set label position
   */
  bool setLabelPosition(void);

  /**
   * Automatically set label orientation
   */
  void setLabelOrientation(void);
  /*------------------------------------------------------------------*/
  LabelPositioner * m_pPositioner;
  /*------------------------------------------------------------------*/



} ;

}

#endif /* _DRAWABLE_LABEL_H_  */
