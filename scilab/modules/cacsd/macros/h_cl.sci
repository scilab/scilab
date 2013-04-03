// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Aclosed=h_cl(P,r,K)
//[Aclosed]=h_cl(P,r,K)
//Given the standard plant P (with r=size(P22)) and the controller
//K (computed e.g. by h_inf) this macro returns the closed loop
//matrix Aclosed whose poles allows to checks the internal stability
//of the closed loop system.
//Aclosed is the A matrix of [I -P22;-K I]^-1;
//!

  if and(typeof(P)<>['rational','state-space']) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"h_cl",1))
  end

  if typeof(P)=="rational" then P=tf2ss(P);end
  [LHS,RHS]=argn(0);
  if RHS==2 then //h_cl(P,K)
    K=r;[A,B2,C2,D22]=abcd(P);
    iK=2
  elseif RHS==3 then //h_cl(P,r,K)
    if typeof(r)<>"constant"|~isreal(r) then
      error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"h_cl",2))
    end
    if size(r,'*')<>2 then
      error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"h_cl",2,2))
    end
    r=int(r);
    if or(r<=0) then
      error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"h_cl",2,2))
    end

    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
    iK=3
  end
  if and(typeof(K)<>['rational','state-space']) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"h_cl",iK))
  end

  if typeof(K)=="rational" then K=tf2ss(K);end
  
  [Ac,Bc,Cc,Dc]=abcd(K);
  [n,pp]=size(B2);[ndc1,ndc2]=size(Dc);[nd1,nd2]=size(D22);
  [m,q]=size(Bc);
  Bw=[B2,0*ones(n,ndc2);
      0*ones(m,pp),Bc];
  [n1,m2]=size(Cc);
  [n2,m1]=size(C2);
  Cw=[0*ones(ndc1,m1),Cc;
      C2,0*ones(n2,m2)];
  Aclosed=[A, 0*ones(n,m);
           0*ones(m,n),Ac]+...
          Bw*inv([eye(ndc1,ndc1),-Dc;-D22,eye(nd1,nd1)])*Cw;
endfunction
