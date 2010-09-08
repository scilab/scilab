
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [u,c]=givens(x,y)
//Syntax : u=givens(xy)
//         u=givens(x,y)
//
// xy = [x;y], u=givens(xy)
// returns a 2*2 matrix u such that u*xy=[r;0].
// c is equal to u*xy
// givens(x,y)=givens([x;y])
//
//!
  [lhs,rhs]=argn(0);
  if rhs==2 then x=[x;y];end
  if or(size(x)<>[2 1]) then 
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A column vector expected.\n"),'givens',1));
  end
  if x(2)<>0 then
    r = norm(x);
    u = [x'; -x(2) x(1)]/r;
    c = [r; 0];
  else
    u=eye(2,2)
    c=x
  end
endfunction
