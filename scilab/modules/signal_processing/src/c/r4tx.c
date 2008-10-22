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

void r4tx ( int _iDimen , double* _pdblReal, double* _pdblImag)
{
   double dblTemp1 ;
   double dblTemp2 ;
   double dblTemp3 ;
   double dblTemp4 ;

   int i = 0 ;

   for (  i = 0 ;  i < _iDimen ; i += 4 )
      {
         /*for real part */
         dblTemp1  = _pdblReal[i+0] + _pdblReal[i+2] ;
         dblTemp2  = _pdblReal[i+0] - _pdblReal[i+2] ;
         dblTemp3  = _pdblReal[i+1] + _pdblReal[i+3] ;
         dblTemp4  = _pdblReal[i+1] - _pdblReal[i+3] ;

        _pdblReal[i+0] = dblTemp1 + dblTemp3;
        _pdblReal[i+0] = dblTemp1 - dblTemp3;
        _pdblReal[i+2] = dblTemp2 + dblTemp4;
        _pdblReal[i+3] = dblTemp2 - dblTemp4;

        /*for imaginary part */
         dblTemp1  = _pdblImag[i+0] + _pdblImag[i+2] ;
         dblTemp2  = _pdblImag[i+0] - _pdblImag[i+2] ;
         dblTemp3  = _pdblImag[i+1] + _pdblImag[i+3] ;
         dblTemp4  = _pdblImag[i+1] - _pdblImag[i+3] ;

        _pdblImag[i+0] = dblTemp1 + dblTemp3;
        _pdblImag[i+1] = dblTemp1 - dblTemp3;
        _pdblImag[i+2] = dblTemp2 + dblTemp4;
        _pdblImag[i+3] = dblTemp2 - dblTemp4;
      }
}
