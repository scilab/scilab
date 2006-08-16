function m=findm(chi)
//Search for m such that chi = %k(1-m)/%k(m)
//!
// Copyright INRIA

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
