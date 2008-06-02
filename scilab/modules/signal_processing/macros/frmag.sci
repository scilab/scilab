function [xm,fr]=frmag(num,den,npts)
//[xm,fr]=frmag(num[,den],npts)
//Calculates the magnitude of the frequency respones of
//FIR and IIR filters.  The filter description can be
//one or two vectors of coefficients, one or two polynomials,
//or a rational polynomial.
//Case 1 (When den is not given):
//  num  :Vector coefficients/Polynomial/Rational
//       :polynomial of filter
//Case 2 (When den is given):
//  num  :Vector coefficients/Polynomial of filter numerator
//  den  :Vector coefficients/Polynomial of filter denominator
//Case 1 and 2:
//  npts :Number of points desired in frequency response
//  xm   :Magnitude of frequency response at the points fr
//  fr   :Points in the frequency domain where
//       :magnitude is evaluated
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

   [lhs,rhs]=argn(0);
   if rhs==2 then,
      npts=den;
   end,
   fr=(0:.5/(npts-1):.5);
   dfr=exp(2*%i*%pi*fr);
   if rhs==2 then,
     
     //-compat type(num)==15 retained for list/tlist compatibility
      if type(num)==15|type(num)==16 then,
         xm=abs(freq(num(2),num(3),dfr));
      else if type(num)==2 then,
         xm=abs(freq(num,poly(1,'z','c'),dfr));
      else if type(num)==1 then,
         xz=poly(num,'z','c');
         xm=abs(freq(xz,1,dfr))
      else,
         error(msprintf(gettext("%s: Wrong type for input argument(s).\n"),'frmag'));
      end,
      end,
      end,
   else if rhs==3 then,
      if type(num)==2 then,
         xm=abs(freq(num,den,dfr));
      else if type(num)==1 then,
         nz=poly(num,'z','c');
         dz=poly(den,'z','c');
         xm=abs(freq(nz,dz,dfr));
      else,
         error(msprintf(gettext("%s: Wrong type for input argument(s).\n"),'frmag'));
      end,
      end,
   end,
   end
endfunction
