function f=%s_l_p(m,p)
// m\p with p matrix of polynomials m matrix of scalar
//!
// Copyright INRIA
[l,c]=size(m)
[mp,np]=size(p);
if l==c then 
  f=inv(m)*p
else
  s=poly(0,varn(p))
  f=m\coeff(p,0)
  for k=1:maxi(degree(p))
    f=f+(m\coeff(p,k))*(s^k)
  end
end
endfunction
