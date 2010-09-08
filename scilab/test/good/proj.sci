
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [y]=proj(x1,x2)
//[y]=proj(x1,x2)  projection on x2 parallel to x1
//F.D.
//!
  [l,k]=size(x1);
  [w,n]=rowcomp(x1);w1=w(:,1:n);
  x1t=w(n+1:l,:);
  y=x2/(x1t*x2)*x1t
endfunction
