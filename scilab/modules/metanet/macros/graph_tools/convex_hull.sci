
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [nhull,ind]=convex_hull(xy)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check xy
  n1=size(xy,1);
  if (n1<>2) then
    error('The input array must be a 2 x n matrix')
  end;
  n=size(xy,2);
  nn=2*n;
  [nhull,iwork]=m6hullcvex(n,nn,xy);
  ind=iwork(1:nhull);
endfunction
