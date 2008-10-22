function [a,b,c]=minreal(a,b,c,domaine,tol)
//
// Copyright INRIA
[lhs,rhs]=argn(0)
select type(a)
case 16
  flag=a(1)
  if flag(1)<>'lss' then error(91,1),end
  if lhs<>1 then error('output: sle'),end;
  select rhs
  case 1 then istol=0
  case 2 then istol=1,tol=b,
  else error('2 inputs to minreal: sl [,tol]'),
  end;
  [a,b,c,d,x0,dom]=a(2:7);
  if dom==[] then error(96,1),end
  domaine='c';if dom<>'c' then domaine='d',end
case 1
  if lhs<>3 then
    error('3 outputs to minreal: ae,be,ce'),
  end;
  select rhs
  case 4 then istol=0
  case 5 then istol=1,
  else error('4 or 5 outputs :a,b,c,domaine [,tol]'),
  end;
else 
  error(91,1)
end;
//
wc=lyap(a',-b*b',domaine);
wo=lyap(a,-c'*c,domaine);
if istol==0 then 
  [r,n]=equil1(wc,wo);
else 
  [r,n]=equil1(wc,wo,tol);
end;
n1=n(1);
ri=inv(r);r=r(1:n1,:);ri=ri(:,1:n1)
a=r*a*ri;b=r*b;c=c*ri
if lhs==1 then a=syslin(dom,a,b,c,d,r*x0),end
endfunction
