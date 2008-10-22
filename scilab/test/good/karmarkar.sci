function [x1,crit]=karmarkar(a,b,c,x0,eps,Gamma)
// Copyright INRIA
maxiter=200;
epsdef=1.d-5
//
[lhs,rhs]=argn(0)
select rhs
case 4 then
  Gamma=1/4,eps=epsdef
case 5 then
  Gamma=1/4
case 6,
else
 error('[x1,crit]=karmarkar(a,b,c,x0 [,eps [,Gamma]])')
end
//verification des donnees
[n,p]=size(a)
w=size(b)
if w(1)<>n then error('invalid B dimension'),end
w=size(c)
if w(1)<>p then error('invalid A dimension'),end
w=size(x0)
if w(1)<>p then error('invalid x0 dimension'),end
if mini(x0)<0|norm(a*x0-b)>eps then 
  error('x0 is not feasible'),
end
//
x1=x0;tc=c';
crit=tc*x1;
test=eps+1
count=0
while test>eps&count<=maxiter
    count=count+1;
//    ax=a*diag(x1);
    ax=a.*(ones(size(a,1),1)*x1');
    xc=x1.*c;
    y=ax'\xc;
//    y=(ax*ax')\(ax*xc)
    d=-xc+ax'*y;
    dk=x1.*d;
    if mini(dk)>0 then error('Unbounded problem!'),end
    alpha=-Gamma/mini(d);
    test=alpha*(norm(d)**2)/maxi(1,abs(crit));
    x1=x1+alpha*dk;
    crit=tc*x1;
    write(%io(2),[count,crit,test],'(f3.0,3x,e10.3,3x,e10.3)')
end
  
endfunction
