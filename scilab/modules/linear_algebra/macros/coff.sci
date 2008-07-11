
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [n,d]=coff(m,var)
//[N,d]=coff(M [,var]) computes (s*eye-M)^-1 
// N = numerator polynomial matrix 
// d = common denominator
// var character string ('s' if omitted)
// See also : coffg
//!
//
  if type(m)<>1 then error(53,1),end
  if m==[] then n=[];d=1;end
  [lhs,rhs]=argn(0);if rhs==1 then var='s',end
  d=clean(poly(m,var)); // denominator
  [n1,n1]=size(m);
  for k=1:n1,for l=1:n1,
      mlk=-m(l,k);
      if abs(mlk)<1 then mlk=1,end
      m(l,k)=m(l,k)+mlk;
      n(k,l)=-(poly(m,var)-d)/mlk;
      m(l,k)=m(l,k)-mlk
    end;end
    if norm(imag(m),1)==0 then n=real(n);d=real(d);end
    n=clean(n);
endfunction
