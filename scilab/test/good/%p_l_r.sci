// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f1=%p_l_r(p1,f1)
//  f1=p1\f1
//!

if size(p1,'*')<>1 then 
  f1=invr(p1)*f1
else
  [n1,d1]=simp(f1('num'),p1*f1('den')),
  f1=rlist(n1,d1,f1('dt'))
end
endfunction
