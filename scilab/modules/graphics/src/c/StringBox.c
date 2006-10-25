/*------------------------------------------------------------------------*/
/* file: StringBox.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#include "StringBox.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "Axes.h"

/*----------------------------------------------------------------------------------------*/
void callXstringL( char * string, int posX, int posY, int boundingRect[4] )
{
  sciPointObj * parentSubWin = sciGetSelectedSubWin ( sciGetCurrentFigure () ) ;

  updateScaleIfRequired( parentSubWin ) ;

  /* now we can call xstringl */
  C2F(dr)("xstringl",string,&posX,&posY,boundingRect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

}
/*----------------------------------------------------------------------------------------*/
