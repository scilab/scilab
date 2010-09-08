// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function MM=unpack(M,blck_szs)
//utility function (for use with semidef)
MM=[]
[mM,nM]=size(M)
n=sum(blck_szs)
for j=1:nM
ptr=1
Mu=[]
for ni=blck_szs
  Mui=[]
  for l=1:ni
    Mui(l:ni,l)=M(ptr:ptr+ni-l,j)
    Mui(l,l:ni)=M(ptr:ptr+ni-l,j)'
    ptr=ptr+ni-l+1
  end
  Mu=[Mu;matrix(Mui,ni*ni,1)]
end
  MM=[MM,Mu]
end
endfunction
