// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [m,n,nx]=%r_size(x,flag)
// only to be called by size function for dynamical systems 
//!

[lhs,rhs]=argn(0)
x1=x(1);
if lhs==1 then 
  if rhs==1 then
    m=size(x('num'));
  else
    m=size(x('num'),flag);
  end
elseif lhs==2 then 
  if rhs<>1 then error(41),end
  [m,n]=size(x('num'));
end
endfunction
