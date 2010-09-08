// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function a=%cblock_c_cblock(a,b)
  na=length(a)
  if na==1 then a=b,return,end
  if length(b)==1 then return,end
  v1=getfield(na,a)
  v2=getfield(2,b)
  
  if size(v1,1)<>size(v2,1) then error(5),end
  if type(v1)==type(v2) then
    setfield(na,[v1 v2],a)
  else
    setfield(na+1,v2,a);na=na+1
  end
  
  for k=3:length(b)
    setfield(na+1,getfield(k,b),a)
    na=na+1
  end
endfunction
