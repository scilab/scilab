// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%p_d_p(p1,p2)
//f= %p_d_p(p1,p2) <=> f=p1./p2 
//!
// Copyright INRIA
if size(p1,'*')==1 then
  p1=p1*ones(p2)
elseif size(p2,'*')==1 then
  p2=p2*ones(p1)
end
[p1,p2]=simp(p1,p2);
f=tlist(['r','num','den','dt'],p1,p2,[])
endfunction
