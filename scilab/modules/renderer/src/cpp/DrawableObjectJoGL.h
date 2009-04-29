/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Objects which can be drawn using JoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_OBJECT_JOGL_H_
#define _DRAWABLE_OBJECT_JOGL_H_

#include "DrawableObjectBridge.h"
#include "DrawableObject.h"
#include "DrawableObjectJavaMapper.hxx"

#ifdef _MSC_VER
#pragma warning(once : 4250)
#endif


namespace sciGraphics
{
class DrawableObjectJoGL : public virtual DrawableObjectBridge
{
public:
  
  /**
   * Constructor
   * @param className name of the Java class which implements the routines
   */
  DrawableObjectJoGL( DrawableObject * drawer ) ;
  
  /**
   * Delete the Java instance used to implement the class 
   */
  virtual ~DrawableObjectJoGL( void ) ;

  /**
   * Initialize the context for drawing (incl starting recording display list)
   */
  virtual void initializeDrawing( void ) ;

  /**
   * Close drawing session (incl stop recording display list)
   */
  virtual void endDrawing( void ) ;

  /**
   * Translate the object with the record move in camera coordinates
   */
  virtual void translate(const double translation[3]);

  /**
   * Translate the object with the record move in camera coordinates
   */
  virtual void endTranslate(void);

  /**
   * Call the display list of an object
   */
  virtual void show( void ) ;

  /**
   * To retrieve the data from the drawer, specially the sciPointObj/
   */
  DrawableObject * getDrawer( void ) { return m_pDrawer ; }

  /**
   * Set the Java mapper to use
   */
  void setJavaMapper( DrawableObjectJavaMapper * javaMapper ) ;

protected:
  
  /**
   * Get the object performing mapping with Java class.
   */
  DrawableObjectJavaMapper * getJavaMapper(void) { return m_pJavaMapper; }

  /*-----------------------------------------------------------------------------*/
  /**
   * Free the Java resources used by this object
   */
  virtual void destroy( void ) ;
  /*-----------------------------------------------------------------------------*/
  /** To retrieve the data from the drawer */
  DrawableObject * m_pDrawer;

  /** Interface for Java Methods */
  DrawableObjectJavaMapper * m_pJavaMapper;
  /*-----------------------------------------------------------------------------*/

};
}

#endif /* _DRAWABLE_OBJECT_JOGL_H_ */

