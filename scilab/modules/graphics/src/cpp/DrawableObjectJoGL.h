/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Objects which can be drawn using JoGL                           */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_JOGL_H_
#define _DRAWABLE_OBJECT_JOGL_H_

#include <jni.h>
#include "DrawableObjectImp.h"

namespace sciGraphics
{
  class DrawableObjectJoGL : virtual public DrawableObjectImp
  {
  public:
    
    /**
     * Constructor
     * @param className name of the Java class which implements the routines
     */
    DrawableObjectJoGL( const char * className ) ;
    
    /**
     * Delete the Java instance used to implement the class 
     */
    virtual ~DrawableObjectJoGL( void ) ;

  protected:
    
    /**
     * Default constructor
     */
    DrawableObjectJoGL( void ) ;
    /*-----------------------------------------------------------------------------*/
    /**
     * Initialize the context for drawing (incl starting recording display list)
     * @param figureIndex index of parentFigure
     */
    virtual void initializeDrawing( int figureIndex ) ;

    /**
     * Close drawing session (incl stop recording display list)
     */
    virtual void endDrawing( void ) ;

    /**
     * Initialize the context for showing (ie displaying recorded object)
     * @param figureIndex index of parentFigure
     */
    virtual void initializeShowing( int figureIndex ) ;

    /**
     * Close showing session
     */
    virtual void endShowing( void ) ;

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

  };
}

#endif /* _DRAWABLE_OBJECT_JOGL_H_ */

