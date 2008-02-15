// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function i1=%i_m_i(i1,i2)
  t1=inttype(i1); t2=inttype(i2);
  //logical promotions
  if t1<10 & t2<10 then t=max(t1,t2); end
  if t1>10 & t2>10 then t=max(t1,t2); end
  //more arbitrary promotions
  if t1<10 & t2>10 then t=max(t1+10,t2); end
  if t1>10 & t2<10 then t=max(t1,t2+10); end
  i1=iconvert(i1,t) * iconvert(i2,t)
endfunction
