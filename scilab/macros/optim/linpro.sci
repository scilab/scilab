function [x,lagr,f]=linpro(p,C,b,ci,cs,mi,x0,imp)
// Copyright INRIA
[lhs,rhs]=argn(0)
n=maxi(size(p));Q=0*ones(n,n);
select rhs
 case 8 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0,imp)
 case 7 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0)
 case 6
[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi)
 case 5 then
[x,lagr,f]=quapro(Q,p,C,b,ci,cs)
 case 4 then
[x,lagr,f]=quapro(Q,p,C,b,ci)
 case 3 then
[x,lagr,f]=quapro(Q,p,C,b)
 else
error('Bad call to linpro')
end
