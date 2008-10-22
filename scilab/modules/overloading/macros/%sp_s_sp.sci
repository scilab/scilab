// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s=%sp_s_sp(a,b)
// %spas - adds 2 sparse matrices (special cases)
//!

if size(a)==[1 1] then
  a=full(a)
  if a==0 then 
    s=b,
  else
    s=a-full(b)
  end
elseif size(b)==[1 1] then
  b=full(b)
  if b==0 then
    s=a
  else
    s=full(a)-b
  end
end
endfunction
