// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%p_l_p(p2,p1)
// f=%p_l_p(p2,p1) <=> f=p2\p1 for polynomial matrices
//!
[l,c]=size(p2)
if l*c <>1 then f=invr(p2)*p1,return,end
[l,c]=size(p1)
[p1,p2]=simp(p1,p2*ones(l,c))
f=tlist(['r','num','den','dt'],p1,p2,[])
endfunction
