// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%r_e(i,j,f)
// %r_e(i,j,f) extraction f(i,j) in a rational matrix
//!

[lhs,rhs]=argn(0)
if rhs==2 then 
  f=rlist(j('num')(i),j('den')(i),j('dt'))
elseif rhs==3 then
  f=rlist(f('num')(i,j),f('den')(i,j),f('dt'))
end
if f('num')==[] then f=[],end
endfunction
