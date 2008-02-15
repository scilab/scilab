// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=%sp_inv(a)

[ma,na]=size(a)
if ma<>na then error(20,1),end
[hand,rk]=lufact(a)
if rk<na then ludel(hand);error(19),end
x=[]
for k=1:ma
  b=0*ones(ma,1);b(k)=1;
  x=[x,sparse(lusolve(hand,b))]
end
ludel(hand);
endfunction
