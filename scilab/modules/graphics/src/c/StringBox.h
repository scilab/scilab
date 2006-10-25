/*------------------------------------------------------------------------*/
/* file: StringBox.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains a set of functions to compute the bounding box of a    */
/*        text                                                            */
/*------------------------------------------------------------------------*/

#ifndef _STRING_BOX_H_
#define _STRING_BOX_H_

/**
* This is a wrapping of the function C2F(dr)("xstringl",... It should be prefered to xstringl
* because it is working even in drawlater mode. The parent Subwindow of the text must be the
* current axes.
*/
void callXstringL( char * string, int posX, int posY, int boundingRect[4] ) ;

#endif /* _STRING_BOX_H_ */