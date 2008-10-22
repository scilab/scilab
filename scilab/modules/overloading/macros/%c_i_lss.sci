// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s2=%c_i_lss(i,j,s1,s2)

if type(i)==10 then  // sl('dt')
  [lhs,rhs]=argn(0)
  if rhs<>3 then  error(21),end
  if i<>'dt' then
    error('inserted element '+i+' has inconsistent type')
  end
  s2=s1;kf=7
  if j<>'c'&j<>'d' then 
    error('inserted element '+i+' must be ''c'' or ''d'' or a scalar')
  end
  s2(kf)=j
  return
end
endfunction
