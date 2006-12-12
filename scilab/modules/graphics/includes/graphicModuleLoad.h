/*------------------------------------------------------------------------*/
/* file: graphicModuleClose.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_MODULE_CLOSE_H_
#define _GRAPHIC_MODULE_CLOSE_H_

/**
 * Routine creating all needed objetcs for the module
 */
void loadGraphicModule( void ) ;

/**
 * Closing function freeing all memory used by the graphic module
 */
void closeGraphicModule( void ) ;

#endif /* _GRAPHIC_MODULE_CLOSE_H_ */
