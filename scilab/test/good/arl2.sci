// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC - JPC
// Copyright (C) INRIA - Serge Steer , Francois Delebecque
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function   [den,num,err]=arl2(y,den0,n,imp,all)
[lhs,rhs]=argn(0);
// test the system type 'c' 'd' or dt 
if rhs <= 4 then all='one';end
if rhs <= 3 then imp=0;end
if all=='all' then
  [den,num,err]=arl2_ius(y,den0,n,imp,all);
else
  [den,num,err]=arl2_ius(y,den0,n,imp);
end;
if lhs<=1 then
  den=syslin('d',num,den);
end
endfunction
