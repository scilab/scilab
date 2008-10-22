/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy interface for the drawArc algorithm  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_ARC_STRATEGY_H_
#define _DRAW_ARC_STRATEGY_H_

namespace sciGraphics
{

class DrawableArc;

class DrawArcStrategy
{

public:

  DrawArcStrategy( DrawableArc * arc ) { m_pDrawed = arc ; }
  
  virtual ~DrawArcStrategy( void ) {}


  /**
   * Main algorithm
   */
  virtual void drawArc( void ) = 0 ;

  /**
   * Recreate display list from precomputed data.
   */
  virtual void redrawArc(void) = 0;

  /**
   * Call the display list of an object
   */
  virtual void showArc( void ) = 0;

protected:

  DrawableArc * m_pDrawed;

};

}

#endif /* _DRAW_ARC_STRATEGY_H_ */
