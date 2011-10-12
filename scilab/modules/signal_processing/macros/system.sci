// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1988 - INRIA - C. Bunks
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [x1, y] = system(x0, f, g, h, q, r)
//<x1,y>=system(x0,f,g,h,q,r)
//define system macro which generates the next
//observation given the old state
//  x0 :Input state vector
//  f  :System matrix
//  g  :Input matrix
//  h  :Output matrix
//  q  :Input noise covariance matrix
//  r  :Output noise covariance matrix
//  x1 :Output state vector
//  y  :Output observation
//System recursively calculates
//
//     x1=f*x0+g*u
//      y=h*x0+v
//
//where u is distributed N(0,q)
//and v is distribute N(0,r).

  [lhs, rhs] = argn(0);
  if rhs == 0 then
    error(999, msprintf(_("%s: Wrong number of input argument(s).\n"), "system"));
  end

  rand('normal');
  q2 = chol(q);
  r2 = chol(r);
  u = q2' * rand(ones(x0));
  v = r2' * rand(ones(x0));
  x1 = f * x0 + g * u;
  y = h * x0 + v;
endfunction
