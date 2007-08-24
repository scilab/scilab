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

/**
 * same as ChoixFormatE when numbers are given through an 
 * array xx[0:nx-1];
 */
void ChoixFormatE1(char *fmt, double *xx, integer nx) ;

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
void ChoixFormatE(char *fmt, double xmin, double xmax, double xpas) ;

double * ReBuildTicksLog2Lin(char logflag, int nbtics, double *grads) ;

int ComputeXIntervals( sciPointObj * pobj, char xy_type, double ** vector, int * N, int checkdim ) ;

void correctBounds( double min, double max, double * lBound, double * uBound );
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

int TheTicks( double * xminv ,
              double * xmaxv , 
              double * grads , 
              int    * ngrads,
              int      compNgrads ) ;

int GradEqual( const double grads[], const int * ngrads) ;

char * getFPF(void);

#endif /* _FORMAT_H_ */
