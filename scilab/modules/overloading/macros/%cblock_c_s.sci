// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function a=%cblock_c_s(a,b)
  if b==[] then return,end
   v=getfield($,a)
  
  if size(v,1)<>size(b,1) then error(5),end
  if type(b)==type(v) then
    setfield($,[v b],a)
  else
    setfield($+1,b,a)
  end
endfunction
