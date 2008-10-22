// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%sp_triu(a,k)

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end

[ij,v,sz]=spget(a)
m=sz(1);n=sz(2)
l=find(ij(:,1)<=(ij(:,2)-k))
d=sparse(ij(l,:),v(l),[m,n])
endfunction
