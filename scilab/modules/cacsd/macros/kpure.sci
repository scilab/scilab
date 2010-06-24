// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [K,R]=kpure(sl,eps)
//if sl is a transfert function N(S)/D(s) kpure looks for K producing
//pure  imaginary roots for
//  P(s)=D(s)+K*N(s)
//There is a pair of pure imaginary poles if and only if
//  P(%i*q)=0  (1)
// and
//  P(-%i*q)=0 (2)
// because N and D are polynomials with real coefficients.

//Author: Serge Steer, INRIA
  y=[];R=[];
  msg=_("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n")
  if argn(2)==1 then eps=1e-6,end
  if size(eps,'*')==2 then  eps=eps(2),end //compatibility
  select typeof(sl)
  case 'rational' then
    if size(sl.num,'*') <> 1 then
      error(msprintf(msg,"kpure",1))
    end
  case 'state-space' then
    if size(sl.D,'*') <> 1 then
      error(msprintf(msg,"kpure",1))
    end
    sl=ss2tf(sl)
  else
    error(msprintf(msg,"kpure",1))
  end

  //(1) give K(s)=-D(s)/N(s)
  s=poly(0,varn(sl.den))
  K=-sl.den/sl.num;
  // replace K by the previous value in (2) and find the roots
  s=roots(numer(horner(sl.den,-s)+K*horner(sl.num,-s)),'e');
  //retain pure imaginary roots
  s=imag(s(abs(real(s))<eps));
  R=(s(s>0).'*%i);
  //find the K(s) values K(s)=-D(s)/N(s)
  K=-real(freq(sl.den,sl.num,R))
endfunction
