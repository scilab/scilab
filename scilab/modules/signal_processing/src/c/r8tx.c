/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "gw_signal.h"


void r8tx ( int _iTempDimen , int _iDimen , int _iLengt ,  double* _pdblReal, double* _pdblImag )
{
    int j = 0 ;
    int i = 0 ;
    int k = 0 ;


     double* pdblReal0 = _pdblReal + ( 0*_iTempDimen );
     double* pdblReal1 = _pdblReal + ( 1*_iTempDimen );
     double* pdblReal2 = _pdblReal + ( 2*_iTempDimen );
     double* pdblReal3 = _pdblReal + ( 3*_iTempDimen );
     double* pdblReal4 = _pdblReal + ( 4*_iTempDimen );
     double* pdblReal5 = _pdblReal + ( 5*_iTempDimen );
     double* pdblReal6 = _pdblReal + ( 6*_iTempDimen );
     double* pdblReal7 = _pdblReal + ( 7*_iTempDimen );

     double* pdblImag0 = _pdblImag + ( 0*_iTempDimen );
     double* pdblImag1 = _pdblImag + ( 1*_iTempDimen );
     double* pdblImag2 = _pdblImag + ( 2*_iTempDimen );
     double* pdblImag3 = _pdblImag + ( 3*_iTempDimen );
     double* pdblImag4 = _pdblImag + ( 4*_iTempDimen );
     double* pdblImag5 = _pdblImag + ( 5*_iTempDimen );
     double* pdblImag6 = _pdblImag + ( 6*_iTempDimen );
     double* pdblImag7 = _pdblImag + ( 7*_iTempDimen );

   double arg,c1,s1,c2,s2,c3,s3,c4,s4,c5,s5,c6,s6,c7,s7 ;
   double dblAReal0, dblAReal1, dblAReal2, dblAReal3, dblAReal4, dblAReal5, dblAReal6, dblAReal7;
   double dblAImag0, dblAImag1, dblAImag2, dblAImag3, dblAImag4, dblAImag5, dblAImag6, dblAImag7 ;

   double dblBReal0, dblBReal1, dblBReal2, dblBReal3, dblBReal4, dblBReal5, dblBReal6, dblBReal7;
   double dblBImag0, dblBImag1, dblBImag2, dblBImag3, dblBImag4, dblBImag5, dblBImag6, dblBImag7 ;

   double dblTReal , dblTImag ;

   double dblPi2 = 8 * atan (1);
   double dblP7  = 1 / sqrt (2) ;
   double scale  = dblPi2 /_iLengt ;



   for ( j = 0 ; j < _iTempDimen ; i++)
      {
       arg=j*scale;

       c1= cos(arg);
       s1= sin(arg);

       c2= c1*c1 - s1*s1 ;
       s2= c1*s1 + c1*s1;

       c3= c1*c2 - s1*s2;
       s3= c2*s1 + s2*c1;

       c4= c2*c2 - s2*s2 ;
       s4= c2*s2 + c2*s2;

       c5= c2*c3 - s2*s3;
       s5= c3*s2 + s3*c2;

       c6= c3*c3 - s3*s3 ;
       s6= c3*s3 + c3*s3;

       c7= c3*c4 - s3*s4;
       s7= c4*s3 + s4*c3;

       for ( k = j ; k < _iDimen ; k += _iLengt )
         {
         dblAReal0 = pdblReal0[k] + pdblReal4[k];
         dblAReal1 = pdblReal1[k] + pdblReal5[k];
         dblAReal2 = pdblReal2[k] + pdblReal6[k];
         dblAReal3 = pdblReal3[k] + pdblReal7[k];
         dblAReal4 = pdblReal0[k] - pdblReal4[k];
         dblAReal5 = pdblReal1[k] - pdblReal5[k];
         dblAReal6 = pdblReal2[k] - pdblReal6[k];
         dblAReal7 = pdblReal3[k] - pdblReal7[k];

         dblAImag0 = pdblImag0[k] + pdblImag4[k];
         dblAImag1 = pdblImag1[k] + pdblImag5[k];
         dblAImag2 = pdblImag2[k] + pdblImag6[k];
         dblAImag3 = pdblImag3[k] + pdblImag7[k];
         dblAImag4 = pdblImag0[k] - pdblImag4[k];
         dblAImag5 = pdblImag1[k] - pdblImag5[k];
         dblAImag6 = pdblImag2[k] - pdblImag6[k];
         dblAImag7 = pdblImag3[k] - pdblImag7[k];

         dblBReal0 = dblAReal0 + dblAReal2;
         dblBReal1 = dblAReal1 + dblAReal3;
         dblBReal2 = dblAReal0 - dblAReal2;
         dblBReal3 = dblAReal1 - dblAReal3;
         dblBReal4 = dblAReal4 - dblAImag6;
         dblBReal5 = dblAReal5 - dblAImag7;
         dblBReal6 = dblAReal4 + dblAImag6;
         dblBReal7 = dblAReal5 + dblAImag7;

         dblBImag0 = dblAImag0 + dblAImag2;
         dblBImag1 = dblAImag1 + dblAImag3;
         dblBImag2 = dblAImag0 - dblAImag2;
         dblBImag3 = dblAImag1 - dblAImag3;
         dblBImag4 = dblAImag4 + dblAReal6;
         dblBImag5 = dblAImag5 + dblAReal7;
         dblBImag6 = dblAImag4 - dblAReal6;
         dblBImag7 = dblAImag5 - dblAReal7;

         pdblReal0[k] = dblBReal0 + dblBReal1;
         pdblImag0[k] = dblBImag0 + dblBImag1;

         if ( j > 1 )
            {
               pdblReal1[k] = c4*( dblBReal0 - dblBReal1 ) - s4*( dblBImag0 - dblBImag1 );
               pdblImag1[k] = c4*( dblBImag0 - dblBImag1 ) + s4*( dblBReal0 - dblBReal1 );
               pdblReal2[k] = c2*( dblBReal2 - dblBImag3 ) - s2*( dblBImag2 + dblBReal3 );
               pdblImag2[k] = c2*( dblBImag2 + dblBReal3 ) + s2*( dblBReal2 - dblBImag3 );
               pdblReal3[k] = c6*( dblBReal2 + dblBImag3 ) - s6*( dblBImag2 - dblBReal3 );
               pdblImag3[k] = c6*( dblBImag2 - dblBReal3 ) + s6*( dblBReal2 + dblBImag3 );

               dblTReal = dblP7*( dblBReal5 - dblBImag5 );
               dblTImag = dblP7*( dblBReal5 + dblBImag5 );

               pdblReal4[k] = c1*( dblBReal4 + dblTReal ) - s1*( dblBImag4 + dblTImag );
               pdblImag4[k] = c1*( dblBImag4 + dblTImag ) + s1*( dblBReal4 + dblTReal );
               pdblReal5[k] = c5*( dblBReal4 - dblTReal ) - s5*( dblBImag4 - dblTImag );
               pdblImag5[k] = c5*( dblBImag4 - dblTImag ) + s5*( dblBReal4 - dblTReal );

               dblTReal = - dblP7* ( dblBReal7 + dblBImag7);
               dblTImag =   dblP7* ( dblBReal7 - dblBImag7);

               pdblReal6[k] = c3*( dblBReal6 + dblTReal ) - s3*( dblBImag6 + dblTImag );
               pdblImag6[k] = c3*( dblBImag6 + dblTImag ) + s3*( dblBReal6 + dblTReal );
               pdblReal7[k] = c7*( dblBReal6 - dblTReal ) - s7*( dblBImag6 - dblTImag );
               pdblImag7[k] = c7*( dblBImag6 - dblTImag ) + s7*( dblBReal6 - dblTReal );
            }
         else
            {
             pdblReal1[k] = dblBReal0 - dblBReal1;
             pdblImag1[k] = dblBImag0 - dblBImag1;
             pdblReal2[k] = dblBReal2 - dblBImag3;
             pdblImag2[k] = dblBImag2 + dblBReal3;
             pdblReal3[k] = dblBReal2 + dblBImag3;
             pdblImag3[k] = dblBImag2 - dblBReal3;

             dblTReal     = dblP7 * ( dblBReal5 - dblBImag5 );
             dblTImag     = dblP7 * ( dblBReal5 + dblBImag5 );

             pdblReal4[k] = dblBReal4 + dblTReal;
             pdblImag4[k] = dblBImag4 + dblTImag;
             pdblReal5[k] = dblBReal4 - dblTReal;
             pdblImag5[k] = dblBImag4 - dblTImag;

             dblTReal     =  -dblP7 *  ( dblBReal7 + dblBImag7 );
             dblTImag     = dblP7 *    ( dblBReal7 - dblBImag7 );

             pdblReal6[k] = dblBReal6 + dblTReal;
             pdblImag6[k] = dblBImag6 + dblTImag;
             pdblReal7[k] = dblBReal6 - dblTReal;
             pdblImag7[k] = dblBImag6 - dblTImag;

            }

         }

      }


}
