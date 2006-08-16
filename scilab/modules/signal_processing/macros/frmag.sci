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
//author: C. Bunks  date: 2 March 1988
// Copyright INRIA

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
         error('Error---Input arguments wrong data type')
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
         error('Error---Input arguments wrong data type')
      end,
      end,
   end,
   end
endfunction
