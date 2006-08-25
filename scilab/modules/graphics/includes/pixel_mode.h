/*------------------------------------------------------------------------*/
/* file: pixel_mode.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : make the correspondence between pixel mode name in scilab       */
/*        and the corresponding index                                     */
/*------------------------------------------------------------------------*/

#ifndef _PIXEL_MODE_H_
#define _PIXEL_MODE_H_

/**
 * return the number of different pixel drawing modes.
 */
int getNbPixelModes( void ) ;

/**
 * Get the name of pixel mode number index.
 */
char * getPixelMode( int index ) ;

/**
 * Look for a pixel mode of name modeName and return its index.
 * If none has been found, return -1. Another negative value is returned
 * if an error occured.
 */
int getPixelModeIndex( const char * modeName ) ;

#endif
