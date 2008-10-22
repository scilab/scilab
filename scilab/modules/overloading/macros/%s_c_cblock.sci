// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function b=%s_c_cblock(a,b)
  if a==[] then return,end
   v2=getfield(2,b)
  
  if size(a,1)<>size(v2,1) then error(5),end
  if type(a)==type(v2) then
    setfield(2,[a v2],b)
  else
    setfield(0,'cblock',b)
    setfield(2,a,b);
  end
endfunction
