// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function r=%c_a_c(a,b)
if size(b,'*')==1 then
  b=b(ones(a))
elseif size(a,'*')==1 then
  a=a(ones(b))
else
  error(8)
end
r=a+b
endfunction
