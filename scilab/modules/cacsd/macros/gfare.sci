// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Z,H]=gfare(Sl)
//[Z,H]=gfare(Sl)
//Generalized Filter Algebraic Riccati Equation
//X = solution , F = gain
//!
  if argn(2)<1 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),..
                   "gfare",1))
  end

  if typeof(Sl)<>'state-space' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"gfare",1))
  end
  if Sl.dt<>"c" then
    error(msprintf(gettext("%s: Wrong type for argument #%d: In continuous time expected.\n"),"gfare",1))
  end

  [A,B,C,D]=abcd(Sl);
  S=eye()+D'*D;R=eye()+D*D';
  Si=inv(S);Ri=inv(R);
  Ar=A-B*Si*D'*C;
  H=[Ar',-C'*Ri*C;
     -B*Si*B',-Ar];
  Z=ric_desc(H);
  H=-(B*D'+Z*C')*Ri
endfunction
