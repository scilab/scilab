// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [M,sel]=pack(M,blck_szs)
//utility function (for use with semidef)
sel=[]
kk=0
blck_szs=matrix(blck_szs,1,size(blck_szs,'*'))
for ni=blck_szs
 k=kk
 for j=1:ni
   sel=[sel,k+(j:ni)]
   k=k+ni
 end
 kk=kk+ni*ni
end
M=M(sel,:)
endfunction
