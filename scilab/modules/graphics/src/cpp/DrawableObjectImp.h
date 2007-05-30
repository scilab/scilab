/*------------------------------------------------------------------------*/
/* file: DrawableObjectImp.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class for routine independent algorithms                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_IMP_
#define _DRAWABLE_OBJECT_IMP_

namespace sciGraphics
{
  class DrawableObjectImp
  {
  public:
    virtual ~DrawableObjectImp( void ) {}

    /**
    * Initialize the context for drawing (incl starting recording display list)
    * @param figureIndex index of parentFigure
    */
    virtual void initializeDrawing( int figureIndex ) = 0 ;

    /**
    * Close drawing session (incl stop recording display list)
    */
    virtual void endDrawing( void ) = 0 ;

    /**
    * Initialize the context for showing (ie displaying recorded object)
    * @param figureIndex index of parentFigure
    */
    virtual void initializeShowing( int figureIndex ) = 0 ;

    /**
    * Close showing session
    */
    virtual void endShowing( void ) = 0 ;
  protected:
  };
}

#endif /* _DRAWABLE_OBJECT_IMP_ */
