// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%r_e(varargin)
// %r_e(i,j,f) extraction f(i,j) in a rational matrix
//author Serge Steer, INRIA
//!
  f=varargin($)
  num=f.num
  den=f.den
  r=rlist(num(varargin(1:$-1)),den(varargin(1:$-1)),f.dt)
  if r.num==[] then r=[],end
endfunction
