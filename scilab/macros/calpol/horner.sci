function r=horner(p,x)
// horner(P,x) evaluates the polynomial or rational matrix P = P(s) 
// when the variable s of the polynomial is replaced by x
// x can be a scalar or polynomial or rational matrix.
// Example: bilinear transform; Assume P = P(s) is a rational matrix
// then the rational matrix P((1+s)/(1-s)) is obtained by
// horner(P,(1+s)/(1-s));
// To evaluate a rational matrix at given frequencies use 
// preferably the freq primitive ;
// See also: freq, repfreq.
//!
//
// Copyright INRIA
if type(p)==16 then r=horner(p(2),x)./horner(p(3),x),return,end
if type(p)==129 then
  p=horner(p(:),x);p=p(1):p(2):p(3)
end
[m,n]=size(p)
if m==-1 then indef=%t,m=1,n=1,p=p+0;else indef=%f,end
r=[]
for l=1:m
  rk=[]
  for k=1:n
   plk=p(l,k)
   d=degree(plk)
   rlk=coeff(plk,d)*ones(x); // for the case horner(1,x)
   for kk=1:d,
     rlk=rlk.*x+coeff(plk,d-kk);
   end;
   rk=[rk rlk]
  end
  r=[r;rk]
end
if indef then r=r*eye(),end
