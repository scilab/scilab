/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the driver independent algorithms to draw a figure handle  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_FIGURE_H_
#define _CONCRETE_DRAWABLE_FIGURE_H_

#include <list>

#include "DrawableFigure.h"

namespace sciGraphics
{

class ConcreteDrawableFigure : public DrawableFigure
{

public:

  ConcreteDrawableFigure( sciPointObj * pObj );

  virtual ~ConcreteDrawableFigure(void);

  /**
   * Like display but to display only a set of objects
   */
  virtual void drawSingleObjs(std::list<sciPointObj *>& singleObjects);

protected:

  /*------------------------------------------------------------------------*/
  /**
   * check if the auto_redraw option is enable
   */
  virtual bool checkAutoRedraw( void ) ;

  /**
   * Check wether the whole figure and hierechy should be redrawn
   * or just a single object.
   */
  virtual bool isDisplayingSingleObject(void);

  /**
   * Display the single object
   */
  virtual void displaySingleObject(void);
  /*------------------------------------------------------------------------*/
  std::list<sciPointObj *> m_oSingleObjects;
  /*------------------------------------------------------------------------*/

private:

  /**
   * Get he list of all the parent subwin of objects inside pObjs
   */
  std::list<sciPointObj *> getParentSubwinList(std::list<sciPointObj *> pObjs);

  /**
   * Get all the objects within pObjs whose parent subwin is parentSubwin
   */
  std::list<sciPointObj *> getChildrenOfSubwin(sciPointObj * parentSubwin,
                                               std::list<sciPointObj *> pObjs);



} ;

}

#endif /* _CONCRETE_DRAWABLE_FIGURE_H_  */
