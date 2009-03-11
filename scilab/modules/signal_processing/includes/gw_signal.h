/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_signal(void);
/*--------------------------------------------------------------------------*/
int sci_ffir(char *fname,unsigned long fname_len);
int sci_fft(char *fname,unsigned long fname_len);
int sci_corr(char *fname,unsigned long fname_len);
int sci_fiir(char *fname,unsigned long fname_len);
int sci_rpem(char *fname,unsigned long fname_len);
int sci_amell(char *fname,unsigned long fname_len);
int sci_delip(char *fname,unsigned long fname_len);
int sci_remez(char *fname,unsigned long fname_len);
int sci_syredi(char *fname,unsigned long fname_len);

/* commented out by VL 04/03/2009 : should'nt be used outside of the module [ */

/* void dfft2 ( double* a , double* b , int nseg , int n , int nspn , */
/*              int isn , int ierr, int* iw , int lw ); */
/* void dfftbi ( double* a , double* b , int nseg , int n , int nspn , */
/*               int isn , int ierr, int lout , int lnow , */
/*               int lused ,int lmax , int lbook , int* rstak , int* istakk ); */
/* void fft842 ( int _iDirect , int _iDimen , int* _pdblReal , int* _pdblImag , int _err ); */
/* void r2tx ( int _iDimen , double* _pdblReal, double* _pdblImag ); */
/* void r4tx ( int _iDimen , double* _pdblReal, double* _pdblImag) ; */
/* void r8tx ( int _iTempDimen , int _iDimen , int _iLengt ,  double* _pdblReal, double* _pdblImag ); */


/* int dfftmx ( double* _pdblA , double* _pdblB , int _iNtot, int _iN, int _iNspan, */
/*              int _iIsn, int _iM, int _iKt, int* _piWt, int* _piCk, */
/*              int* _piBt, int* _piSk ,int* _piNp, int* _piNfac); */
/* /\* under functions used by dfftmx *\/ */
/*    void preliminaryWork (void); */
/*    void permute_stage1 (void); */
/*    void permute_stage2 (void); */
/*    void f4t_150 (void); */
/*    void factorOf3Transform (void) ; */
/*    void factorOf5Transform (void) ; */
/*    void preFOtherTransform (void); */
/*    void factorOfOtherTransform (void); */
/*    void pre_sqFactor2NormlOrder (void); */
/*    void nonSqFactor2NormOrder (void) ; */
/*    void detPermutCycles (void); */
/*    void reorderMatrix (void ) ; */
   
/*    int f4t_170 (void); */
/*    int factorTransform (void); */
/*    int pre_fOf2Trans (void); */
/*    int factorOf2Transform (void); */
/*    int factorOf4Transform (void); */
/*    int  mulByRotationFactor (void ); */
/*    int  post_sqFactor2NormlOrder (void); */
/*    int  single_sqFactor2NormlOrder (void); */
/*    int  preF2transform (void) ; */
/*    int  multi_sqFactor2NormlOrder (void); */
/*    int  end (void) ; */

/* ] VL 04/03/2009 end of commented out part */

#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

