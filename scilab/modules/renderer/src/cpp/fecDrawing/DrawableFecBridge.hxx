/*------------------------------------------------------------------------*/
/* file: DrawableFecBridge.h                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        Fec object                                                      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_BRIDGE_HXX_
#define _DRAWABLE_FEC_BRIDGE_HXX_

#include "../DrawableObject.h"
#include "../DrawableClippedObjectBridge.h"

namespace sciGraphics
{

/* To avoid cyclic dependencies */
class DrawableFec;

class DrawableFecBridge : public virtual DrawableObjectBridge
{
public:

  DrawableFecBridge( void ) {}

  virtual ~DrawableFecBridge( void ) {}

  /**
  * Return the drawn object
  */
  virtual DrawableFec * getFecDrawer( void ) = 0 ;

};

}

#endif /* _DRAWABLE_FEC_BRIDGE_HXX_ */
