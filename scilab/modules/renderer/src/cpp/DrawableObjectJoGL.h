/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Objects which can be drawn using JoGL                           */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_JOGL_H_
#define _DRAWABLE_OBJECT_JOGL_H_

#include <jni.h>
#include "DrawableObjectBridge.h"
#include "DrawableObject.h"

namespace sciGraphics
{
  class DrawableObjectJoGL : public virtual DrawableObjectBridge
  {
  public:
    
    /**
     * Constructor
     * @param className name of the Java class which implements the routines
     */
    DrawableObjectJoGL( DrawableObject * drawer, const char * className ) ;
    
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
     * Initialize the context for showing (ie displaying recorded object)
     */
    virtual void initializeShowing( void ) ;

    /**
     * Close showing session
     */
    virtual void endShowing( void ) ;

    /**
     * Call the display list of an object
     */
    virtual void show( void ) ;

    /**
     * To retrieve the data from the drawer, specially the sciPointObj/
     */
    virtual DrawableObject * getDrawer( void ) { return m_pDrawer ; }

  protected:
    
    /*-----------------------------------------------------------------------------*/

    /**
     * Free the Java resources used by this object
     */
    virtual void destroy( void ) ;
    /*-----------------------------------------------------------------------------*/
    /** the Java class corresponding to this one */
    jclass m_oDrawableClass ;

    /** the instance of the Java class */
    jobject m_oDrawableObject ;
    /*-----------------------------------------------------------------------------*/
    /** To retrieve the data from the drawer */
    DrawableObject * m_pDrawer;
    /*-----------------------------------------------------------------------------*/

  };
}

#endif /* _DRAWABLE_OBJECT_JOGL_H_ */

