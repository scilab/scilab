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

void fft842 ( int _iDirect , int _iDimen , int* _pdblReal , int* _pdblImag , int _err )
{
   int i  = 0 ;
   int ipass = 1 ;


   int ij ;
   int ji ;
   int j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14;
   int iPow2ofDimen  ;
   int iTempDimen = 1 ;
   int iTemp2Pow2ofDimen ;
   int iPow8ofDimen ;
   int iLengt  ;
   int CRES = 0 ;

   int l[15] ;



   double dblTemp ;

   do
      {
         iPow2ofDimen = i ;
         if ( i != 0)
            iTempDimen  *= 2 ;
         i++;
      }while ( _iDimen != iTempDimen || i < 15 );

   if ( _iDimen != iTempDimen)
      {
      _err = 1 ;
      return ;
      }

   _err = 0 ;

   if ( _iDirect == 0 )
      for ( i = 0 ; i < _iDimen ; i++)
         {
          _pdblImag[i] *= -1 ;
         }

   iPow8ofDimen = iPow2ofDimen / 3 ;
   if ( iPow8ofDimen != 0 )
      {
      /* development of the algo in 8-base if ... */
      for ( ipass = 1 ; i < iPow8ofDimen ; i++ )
         {
           iTemp2Pow2ofDimen = 1 << ( iPow2ofDimen - 3*ipass ) ;
            iLengt = iTemp2Pow2ofDimen * 8 ;
            r8tx( iTemp2Pow2ofDimen, _iDimen, iLengt, _pdblReal, _pdblImag );
         }
      }

   CRES = iPow2ofDimen - 3*iPow8ofDimen - 1 ;

   if ( CRES >=  0)
      {
         if ( CRES == 0 )
             {
               r2tx ( _iDimen , _pdblReal[0] , _pdblReal[1] ,_pdblImag[0] , _pdblImag[1] ) ;

             }
         else
             {
               r4tx ( _iDimen , _pdblReal , _pdblImag) ;
             }
      }

   for ( i = 14 ; i >= 0 ; i-- )
      {
         l[i] = 1 ;
         CRES = i - iPow2ofDimen ;
         if ( CRES <= 0)
            l[i] = 1 << ( iPow2ofDimen + 1 -i ) ;


      }

     ij = 0 ;


      for ( j1=0  ; j1 < l[1-1] ;  j1++ )
      for ( j2=j1 ; j2 < l[2-1] ;  j2 += l[1-1] )
      for ( j3=j2 ; j3 < l[3-1] ;  j3 += l[2-1] )
      for ( j4=j3 ; j4 < l[4-1] ;  j4 += l[3-1] )
      for ( j5=j4 ; j5 < l[5-1] ;  j5 += l[4-1] )
      for ( j6=j5 ; j6 < l[6-1] ;  j6 += l[5-1] )
      for ( j7=j6 ; j7 < l[7-1] ;  j7 += l[6-1] )
      for ( j8=j7 ; j8 < l[8-1] ;  j8 += l[7-1] )
      for ( j9=j8 ; j9 < l[9-1] ;  j9 += l[8-1] )
      for ( j10=j9 ; j10 < l[10-1] ;  j10 += l[9-1] )
      for ( j11=j10 ; j11 < l[11-1] ;  j11 += l[10-1] )
      for ( j12=j11 ; j12 < l[12-1] ;  j12 += l[11-1] )
      for ( j13=j12 ; j13 < l[13-1] ;  j13 += l[12-1] )
      for ( j14=j13 ; j14 < l[14-1] ;  j14 += l[13-1] )
      for ( ji=j14 ; ji < l[15-1] ;  ji += l[14-1]  )
         {
            CRES = ij - ji ;
            if ( CRES < 0 )
               {
                  dblTemp        = _pdblReal[ij];
                  _pdblReal[ij]  = _pdblReal[ji];
                  _pdblReal[ji]  = dblTemp;
                  dblTemp        = _pdblImag[ij];
                  _pdblImag[ij]  = _pdblImag[ji];
                  _pdblImag[ji]  = dblTemp;

               }
            ij ++ ;
         }



   /*130*/
   if ( _iDirect == 0 )
     {
      for ( i = 0 ; i < _iDimen ; i++)
         {
          _pdblImag[i] *= -1 ;
         }
      }
   else
     {
      for ( i = 0 ; i < _iDimen ; i++)
         {
          _pdblReal[i] /= _iDimen ;
          _pdblImag[i] /= _iDimen ;
         }
      }

}