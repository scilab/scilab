// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [N,M]=lcf(Sl)
//Compute Normalized coprime factorization of a linear dynamic system
//%Syntax and parameters description
//     [N,M]=lcf(Sl)
//
//  SL  : linear dynamic system given in state space or transfer function.
//       see syslin
//  N,M : is realization of Sl: Sl = M^-1 N
//!

  if and(typeof(Sl)<>['rational','state-space']) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"lcf",1))
  end
  if Sl.dt<>"c" then 
    error(msprintf(gettext("%s: Wrong type for argument %d: In continuous time expected.\n"),"lcf",1))
  end
  flag=0;
  if typeof(Sl)=='rational' then Sl=tf2ss(Sl),flag=1;end

  [A,B,C,D]=Sl(2:5);[nw,nb]=size(B);[nc,nw]=size(C);
  R=eye()+D*D';
  [Z,H]=gfare(Sl);
  Ar=A+H*C;
  Bn=B+H*D;Bm=H;
  Rm12=inv(sqrtm(R));
  Cr=Rm12*C;Dn=Rm12*D;Dm=Rm12;
  N=syslin('c',Ar,Bn,Cr,Dn);
  M=syslin('c',Ar,Bm,Cr,Dm);
  if flag==1 then N=ss2tf(N);M=ss2tf(M);end
endfunction
