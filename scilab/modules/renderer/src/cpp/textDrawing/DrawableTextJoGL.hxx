/*------------------------------------------------------------------------*/
/* file: DrawableTextJoGL.hxx                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        text object with JoGL                                           */
/*------------------------------------------------------------------------*/

#include "DrawableTextBridge.hxx"
#include "DrawableText.h"
#include "DrawableTextJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableTextJoGL : public DrawableTextBridge, public DrawableClippedObjectJoGL
{

public:
  DrawableTextJoGL( DrawableText * drawer ) ;

  virtual ~DrawableTextJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawableText * getTextDrawer( void ) ;

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableTextJavaMapper * getTextJavaMapper(void);

};

}
