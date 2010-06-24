
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [S]=sqroot(Q)
  Q1=(Q+Q')/2;
  if norm(Q1-Q,1) > 100*%eps then 
    warning(msprintf(gettext("%s: Wrong size for input argument #%d: Symmetric expected"),'sqroot',1));
  end
  tt=min(spec(Q1));
  if tt <-10*%eps then 
    warning(msprintf(gettext("%s: Wrong value for input argument #%d: Not semi-definite positive"),'sqroot',1));
  end
  if norm(Q,1) < sqrt(%eps) then S=[];return;end
  [u,S,v,rk]=svd(Q);
  S=v(:,1:rk)*sqrt(S(1:rk,1:rk));
  if norm(imag(Q1),1) <1.d-8 then S=real(S);;end
endfunction
