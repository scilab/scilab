function [s1]=des2ss(a,b,c,d,e,tol)
//descriptor to state-space
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
 [a,b,c,d,e]=a(2:6);
// if norm(d,1)<>0 then warning('des2ss: d matrix must be zero!');end
 [bfs,bis,chis]=glever(e,a);
 s1=c*tf2ss(bfs/chis)*b;s1(5)=-c*bis*b+d;
return;
end
if rhs == 5 then tol=1.e-8;end
[ns,ns] = size(a);
if norm(e,1) < %eps then s1=syslin([],[],[],[],-c/a*b + d);return;end
[ue,se,ve,rk] = svd(e,tol*norm(e,1));
k=ns-rk;
if k==0 then ei=inv(e),s1=syslin([],ei*a,ei*b,c,d),return,end
u1=ue(:,1:ns-k); u2=ue(:,ns-k+1:ns);
v1=ve(:,1:ns-k); v2=ve(:,ns-k+1:ns);
if k==0 then u2=1,v2=1;end
a22=u2'*a*v2;
if rcond(a22) < 1.d-4 then 
  //    higher index...
  s=poly(0,'s');[ws,fs1]=rowshuff(s*e-a);
  ww=inv(syslin('c',[],[],[],fs1));
  s1=c*ww*ws*b+d;
  return
end
sei=sqrtm(inv(se(1:ns-k,1:ns-k)));
//sei=(se(1:ns-k,1:ns-k))^(-0.5)
a11=u1'*a*v1;
a12=u1'*a*v2;
a21=u2'*a*v1; 
//   index one...
a22i=inv(a22);
bb1 = u1'*b;
bb2 = u2'*b;
cc1 = c*v1;
cc2 = c*v2;
aa = sei * (a11 - a12*a22i*a21) * sei;
bb = sei * (bb1  - a12*a22i*bb2);
cc = (cc1 - cc2 * a22i * a21) * sei;
dd = d - cc2 * a22i * bb2;
s1=syslin([],aa,bb,cc,dd);
endfunction
