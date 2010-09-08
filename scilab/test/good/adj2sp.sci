// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function p=adj2sp(xadj,adjncy,anz,mn)
//adjacency to sparse conversion.
[LHS ,RHS ]=argn(0);

nb=size(xadj,'*')-1;
nza=size(adjncy,'*');
i = fadj2sp(xadj(:),nb,nza);
nr=max(adjncy);
if RHS == 4  then
p=sparse([i,adjncy(:)],anz,[mn(2),mn(1)])';
else
p=sparse([i,adjncy(:)],anz,[nb,nr])';
end;
endfunction
