// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function e=nextpow2(n)
  n=abs(n)
  kf=find(~isnan(n)&n<>%inf)
  e=n;f=zeros(n)
  [f(kf),e(kf)]=frexp(n(kf))
  k=find(f==0.5) // n(k) is a power of 2
  e(k)=e(k)-1
endfunction
