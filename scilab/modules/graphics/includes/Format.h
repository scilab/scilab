/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    Copyright INRIA 2006
*    Jean-Baptiste Silvy
*    Axis drawing for 2d plots (format selection) 
--------------------------------------------------------------------------*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "ObjectStructure.h"
#include "StringMatrix.h"

int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, integer *np1, integer *np2, integer *kminr, integer *kmaxr, integer *ar) ;

int ComputeC_format( sciPointObj * pobj, char * c_format ) ;

int ComputeXIntervals( sciPointObj * pobj, char xy_type, double ** vector, int * N, int checkdim ) ;

StringMatrix * computeDefaultTicsLabels( sciPointObj * pobj ) ;

int ChooseGoodFormat( char * c_format,char logflag, double *_grads,int n_grads ) ;

double * ReBuildTicksLog2Lin(char logflag, int nbtics, double *grads) ;

/**
 * Create a new string which is the result the conversion of a double value
 * using a certain format
 * @param bufferSize size of the buffer used to store the store before the copying
 *                   it to the result. It must greater than the length of the returning string.
 *                   and ideally the same length.
 * @return the newly created strings, or NULL if an error occurred.
 */
char * copyFormatedValue( double value, const char format[5], int bufferSize ) ;

/**
* Create a new string array which is the conversion of the doubles values. 
* @param bufferSize size of the buffer used to store the store before the copying
*                   it to the result. It must greater than the length of any of the strings.
*                   and ideally the maximum length.
*/
char ** copyFormatedArray( const double values[], int nbStrings, const char format[5], int bufferSize ) ;

/**
 * free an array of strings
 */
void destroyStringArray( char * src[], int nbStrings ) ;

#endif /* _FORMAT_H_ */
