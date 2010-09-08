// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function X = lyap(A,C,flag)
//  solve  A'*X+X*A=C if flag=='c' or  A'*X*A-X=C if flag=='d'
  if argn(2)<>3 then error(39),end
  flag=part(flag,1)
  if flag=='c' then 
    flag=[0 0], 
  elseif flag=='d' then
    flag=[1 0],
  else
    error(36,3)
  end
  X=linmeq(2,A,C,flag)
endfunction
