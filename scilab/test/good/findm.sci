// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function m=findm(chi)
//Search for m such that chi = %k(1-m)/%k(m)
//!

if chi < 1 then
  t=1;
  tn=2;
  m=0.99999;
  mn=2;
  v=16*exp(-%pi/chi);
  while abs(t-tn) > 10.*%eps
    t=tn;
    lln=log(16/(1-m));
    k1=%asn(1,1-m);
    k=%asn(1,m);
    y=(k1*lln/%pi)-k;
    mn=m;
    m=1-v*exp((-%pi*y)/k1);
    tn=m+mn;
  end
else
  t=1;
  tn=2;
  m=0.00001;
  mn=0.1;
  v=16*exp(-%pi*chi);
  while abs(t-tn) > 10.*%eps
    t=tn;
    lln=log(16/m);
    k1=%asn(1,1-m);
    k=%asn(1,m);
    y=(k*lln/%pi)-k1;
    mn=m;
    m=v*exp((-%pi*y)/k);
    tn=m+mn;
  end
end
endfunction
