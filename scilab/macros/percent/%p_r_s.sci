function f=%p_r_s(p,m)
// f=%p_r_s(p,m) <=> f=p/m 
// with p matrix of polynomials, m constant matrix
//!
// Copyright INRIA
[l,c]=size(m)
[mp,np]=size(p);
if l==c then 
  f=p*inv(m)
else
  s=poly(0,varn(p))
  f=coeff(p,0)/m
  for k=1:maxi(degree(p))
    f=f+(coeff(p,k)/m)*(s^k)
  end
end
endfunction
