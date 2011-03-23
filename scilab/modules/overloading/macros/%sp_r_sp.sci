// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=%sp_r_sp(a,b)
// a/b , a sparse b sparse

[ma,na]=size(a)
[mb,nb]=size(b)
if na<>nb then error(12),end
if mb<>nb then
  a=a*b';b=b*b'
end

[h,rk]=lufact(b')
if rk<min(mb,nb) then warning('deficient rank: rank = '+string(rk)),end
x=[]    
for k=1:ma
  x=[x;sparse(lusolve(h,full(a(k,:))')')]
end
ludel(h)
endfunction
