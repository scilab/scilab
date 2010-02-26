/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
*    Graphic library
*    Axis drawing for 2d plots (format selection) 
--------------------------------------------------------------------------*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "dynlib_graphics.h"
#include "machine.h" /* C2F */
#include "ObjectStructure.h"
#include "StringMatrix.h"

/**
 * @TODO add comment
 *
 * @param xmi   
 * @param xma   
 * @param xi    
 * @param xa    
 * @param np1   
 * @param np2   
 * @param kminr 
 * @param kmaxr 
 * @param ar    
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, int *np1, int *np2, int *kminr, int *kmaxr, int *ar) ;

/**
 * @TODO add comment
 *
 * @param pobj  
 * @param c_format  
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ComputeC_format( sciPointObj * pobj, char * c_format ) ;

/**
 * @TODO add comment
 *
 * @param pobj  
 * @param xy_type   
 * @param vector    
 * @param N 
 * @param checkdim  
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ComputeXIntervals( sciPointObj * pobj, char xy_type, double ** vector, int * N, int checkdim ) ;

/**
 * @TODO add comment
 *
 * @param pobj  
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP StringMatrix * computeDefaultTicsLabels( sciPointObj * pobj ) ;

/**
 * @TODO add comment
 *
 * @param c_format  
 * @param logflag   
 * @param _grads    
 * @param n_grads   
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ChooseGoodFormat( char * c_format,char logflag, double *_grads,int n_grads ) ;

/**
 * same as ChoixFormatE when numbers are given through an 
 * array xx[0:nx-1];
 */
GRAPHICS_IMPEXP void ChoixFormatE1(char *fmt, double *xx, int nx) ;

/**
 * ChoixFormatE returns a format ("%.*f" or "%.*e")
 * in fmt given xmin,xmax,pas. 
 *   fmt : character string 
 * fmt gives a format which can be used to display
 * number in range xmin:step:xmax  
 * Exemple : ChoixFormatE(format,min,max,step);
 *           fprintf(format,min+k*step);
 * The format is searched so as to give distinct values 
 * for the numeric values xmin + k*xpas in [xmin,xmax] 
 * and give enough precision. 
 */
GRAPHICS_IMPEXP void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas) ;


/**
 * @TODO add comment
 *
 * @param min   
 * @param max   
 * @param lBound    
 * @param uBound    
 */
GRAPHICS_IMPEXP void correctBounds( double min, double max, double * lBound, double * uBound );
/**
 * Create a new string which is the result the conversion of a double value
 * using a certain format
 * @param bufferSize size of the buffer used to store the store before the copying
 *                   it to the result. It must greater than the length of the returning string.
 *                   and ideally the same length.
 * @return the newly created strings, or NULL if an error occurred.
 */
GRAPHICS_IMPEXP char * copyFormatedValue( double value, const char format[5], int bufferSize ) ;

/**
* Create a new string array which is the conversion of the doubles values. 
* @param bufferSize size of the buffer used to store the store before the copying
*                   it to the result. It must greater than the length of any of the strings.
*                   and ideally the maximum length.
*/
GRAPHICS_IMPEXP char ** copyFormatedArray( const double values[], int nbStrings, const char format[5], int bufferSize ) ;

/**
 * @TODO add comment
 *
 * @param xminv 
 * @param xmaxv 
 * @param grads 
 * @param ngrads    
 * @param compNgrads    
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int TheTicks( double * xminv ,
              double * xmaxv , 
              double * grads , 
              int    * ngrads,
              int      compNgrads ) ;

/**
 * @TODO add comment
 *
 * @param _min  
 * @param _max  
 * @param _grads    
 * @param n_grads   
 * @param compNgrads    
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int GradLog( double _min, double _max, double *_grads, int * n_grads, int compNgrads);

/**
 * @TODO add comment
 *
 * @param  const double grads[] 
 * @param ngrads    
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int GradEqual( const double grads[], const int * ngrads) ;

/**
 * @TODO add comment
 *
 * @param void  
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP char * getFPF(void);

GRAPHICS_IMPEXP int sciGetLogExponent( double minBound, double maxBound, double * expMin, double * expMax );

#endif /* _FORMAT_H_ */
