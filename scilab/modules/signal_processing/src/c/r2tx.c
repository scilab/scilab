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

void r2tx ( int _iDimen , double* _pdblReal, double* _pdblImag )
{
   double dblTemp ;


   int i = 0 ;

   for ( i = 0 ; i < _iDimen ;  i += 2 )
      {
         /*for real part */
         dblTemp        = _pdblReal[i] + _pdblReal[i+1] ;
         _pdblReal[i+1] = _pdblReal[i] - _pdblReal[i+1] ;
         _pdblReal[i+0] = dblTemp ;
         /*for imaginary one */
         dblTemp         = _pdblImag[i] + _pdblImag[i+1] ;
         _pdblImag[i+1] = _pdblImag[i] - _pdblImag[i+1] ;
         _pdblImag[i]   = dblTemp ;


      }
}
