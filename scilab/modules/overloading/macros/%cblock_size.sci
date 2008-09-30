// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [m,n]=%cblock_size(M)
  n=size(definedfields(M),'*')-1;
  m=size(getfield(2,M),1);
  n=0
  for k=2:size(definedfields(M),'*')
    n=n+size(getfield(k,M),2);
  end
  if argn(2)==1 then m=[m,n],end
endfunction
