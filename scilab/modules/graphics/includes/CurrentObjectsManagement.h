/*------------------------------------------------------------------------*/
/* file: CurrentObjectsManagement.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Set of functions to set and get current objects                 */
/*------------------------------------------------------------------------*/

#ifndef _GET_CURRENT_OBJECTS_H_
#define _GET_CURRENT_OBJECTS_H_

#include "ObjectStructure.h"

/**
* To know if the current figure is a graphic one.
* @return 
*/
int  get_cf_type( void ) ;

/**
 * @TODO add comment
 *
 * @param val   
 */
void set_cf_type( int val ) ;
/*-----------------------------------------------------------------------------*/
/**
 * Returns the pointer to the current selected figure. 
 */
sciPointObj * sciGetCurrentFigure( void ) ;

/**
 * To know if a figure is the selected one.
 */
BOOL sciIsCurrentFigure(sciPointObj * pFigure);

/**
 * @TODO add comment
 *
 * @param mafigure  
 * @return <ReturnValue>
 */
int sciInitCurrentFigure( sciPointObj * mafigure ) ;

/**
 * @TODO add comment
 *
 * @param mafigure  
 * @return <ReturnValue>
 */
int sciSetCurrentFigure(  sciPointObj * mafigure ) ;
/*-----------------------------------------------------------------------------*/
/**
 * Get the current Object
 */
sciPointObj * sciGetCurrentObj( void ) ;

void sciSetCurrentObj( sciPointObj * pobj ) ;

/**
 * Return the handle on the current object
 */
long sciGetCurrentHandle( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentFrame( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentWindow( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentWindowFrame( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentScreen( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentSubWin( void ) ;
/*-----------------------------------------------------------------------------*/
sciPointObj * sciGetCurrentConsole( void ) ;
/*-----------------------------------------------------------------------------*/

#endif /* _GET_CURRENT_OBJECTS_H_ */
