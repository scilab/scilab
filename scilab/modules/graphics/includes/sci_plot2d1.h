/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_plot2d1.h                                                    */
/* desc : interface for plot2d1, plot2d2, plot2d3 and plot2d4 routines    */
/*------------------------------------------------------------------------*/

#ifndef _INT_PLOT2D1_H_
#define _INT_PLOT2D1_H_

/**
* interface function for the plot2d1 routine.
*
* @param[in] fname     name of the routine (ie plot2d1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d1( char * fname, unsigned long fname_len ) ;

/**
* interface function for the plot2d1_1 routine.
*
* @param[in] fname     name of the routine (ie plot2d1_1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d1_1 (char *fname,unsigned long fname_len);

/**
* interface function for the plot2d1_2 routine.
*
* @param[in] fname     name of the routine (ie plot2d1_2)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d1_2 (char *fname,unsigned long fname_len);

/**
* interface function for the plot2d1_3 routine.
*
* @param[in] fname     name of the routine (ie plot2d1_3)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d1_3 (char *fname,unsigned long fname_len);

/**
* interface function for the plot2d1_4 routine.
*
* @param[in] fname     name of the routine (ie plot2d1_4)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d1_4 (char *fname,unsigned long fname_len);
/**
* interface function for the plot2d2 routine.
*
* @param[in] fname     name of the routine (ie plot2d2)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d2( char * fname, unsigned long fname_len ) ;

/**
* interface function for the plot2d3 routine.
*
* @param[in] fname     name of the routine (ie plot2d3)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d3( char * fname, unsigned long fname_len ) ;

/**
* interface function for the plot2d4 routine.
*
* @param[in] fname     name of the routine (ie plot2d4)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot2d4( char * fname, unsigned long fname_len ) ;

int sci_plot2d1_G( char *fname, int ptype, unsigned long fname_len ) ;

#endif /* _INT_PLOT2D1_H_ */
