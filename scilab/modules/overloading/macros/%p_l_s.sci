// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%p_l_s(p,m)
// f=%p_l_s(p,m) <=>f= p\m.
//!
[mp,np]=size(p)
if mp*np<>1 then 
  f=invr(p)*m,
else
  [l,c]=size(m)
  if m<>[] then m=m+poly(0,varn(p),'c'),end
  if mp==-1&l*c==1|l==-1 then
    f=tlist(['r','num','den','dt'],m,p*eye(),[])
  else
    f=tlist(['r','num','den','dt'],m,p*ones(l,c),[])
  end
end
endfunction
