/*------------------------------------------------------------------------*/
/* file: sci_demo.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : function used to laubch a demo of a command                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_DEMO_
#define _INT_DEMO_

/**
 * Launch a demo of a scilab routine
 * @param[in] fname Came of the routine which demo is launched
 * @param[in] code  Scilab script of the demo
 * @param[in] flagx If *flagx == 1, display the code of the demo in the
 *                  console. Otherwise, silent execution.
 */
int sci_demo( char * fname, char * code, int * flagx ) ;

#endif /* _INT_DEMO_ */
