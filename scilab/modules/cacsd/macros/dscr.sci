function [f,r]=dscr(a,dt,m)
// Copyright INRIA
[lhs,rhs]=argn(0);lst=0
if type(dt)<>1 then
  error('dscr: sampling period must be a positive real scalar')
end
if size(dt,'*')<>1|~isreal(dt)|dt<=0 then
  error('dscr: sampling period must be a positive  real scalar')
end

flag=a(1);
select flag(1)
case 'r' then
  a=tf2ss(a);
  [a,b,c,d,x0,dom]=a(2:7);
case 'lss' then
  [a,b,c,d,x0,dom]=a(2:7)
else 
  error(97,1),
end;
if dom<>'c' then 
  warning('dscr: time domain assumed to be continuous'),
end
[n1,m1]=size(b),
s=expm([a,b;0*ones(m1,n1+m1)]*dt),
f=s(1:n1,1:n1);g=s(1:n1,n1+1:n1+m1);
if rhs==3 then
  s=expm([-a,m;0*a a']*dt),
  r=f*s(1:n1,n1+1:n1+n1),
end;
f=syslin(dt,f,g,c,d,x0)
endfunction
