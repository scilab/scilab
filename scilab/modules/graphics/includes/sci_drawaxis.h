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
/* file: sci_drawaxis.h                                                   */
/* desc : interface for sci_drawaxis routine                              */
/*------------------------------------------------------------------------*/

#ifndef _INT_DRAWAXIS_H_
#define _INT_DRAWAXIS_H_

/**
* interface function for the set_drawaxis routine :
* dir = 'u','r','d','l'  [default -> 'l' ]
* fontsize =             [default -> -1   ]
* format_n = format to use for numbers (unused if strings)
* seg = flag 1 or 0 draw the base segment of the axis (default 1)
* sub_int  = number of sub tics (default 2)
* textcolor =            [default -> -1 ]
* ticscolor =            [default -> -1 ]
* tics = 'v' 'r' 'i'     [default -> 'v' ]
* gives tics type : vector | range | irange (v,r,i)
* val  = string matrix
* x = scalar | vecteur | range | irange 
* y = scalar | vecteur | range | irange
* 
* constraints :
* ------------ 
* dir = 'u' | 'd' ==> y= scalar | [] 
* x= vecteur | range | irange
* dir = 'r' | 'l' ==> x= scalar | []
*                     y= vecteur | range | irange
* tics = 'r'          ==> x or y is of size 3 (according to dir)
* tics = 'i'          ==> x or y is of size 4
* val  =              ==> must be of size compatible with x or y
*                         according to dir
*
* @param[in] fname     name of the routine (ie set_posfig_dim)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_drawaxis( char * fname, unsigned long fname_len ) ;

int check_xy(char *fname, char dir, int mn, int xpos, int xm, int xn, 
             long unsigned int xl, int ypos, int yRow, int yCol, long unsigned int yl, 
             int *ntics) ;

#endif /* _INT_DRAWAXIS_H_ */
