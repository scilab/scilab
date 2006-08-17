function [x]=nehari(r,tol)
// [x]=nehari(R,tol) returns the Nehari approximant of R.
// R = linear system in state-space representation (syslin list)
//- R is strictly proper and - R~ is stable (i.e. R is antistable).
//    || R - X ||oo = min || R - Y ||oo
//                  Y in Hoo
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
r1=r(1);
if r1(1)<>'lss' then error('State-space only!'),end
if r(7)=='d' then error('Continuous-time only!'),end
r(7)='c'
//
if rhs==1 then tol=1e-6,end,
//norm of Hankel operator
//-----------------------
nk=nophkel(r),nn=nk+tol,
r(3)=r(3)/nn,
//best approx.
//------------
xo=-obs_gram(r),xc=-ctr_gram(r),
w=inv(eye()-xo*xc),
[m,k,n]=size(r),m=m(1),
[a,b,c]=r(2:4),o=0*ones(a),
ax=[a,o,o;o,a,-w'*b*b';o,o,-a'-w*xo*b*b'],
bx=[b;w'*b;w*xo*b],cx=[c,-c,0*ones(m,n)],
x=syslin('c',ax,bx,cx*nn),
[y,x]=dtsi(x);


endfunction
function [nk]=nophkel(sl,tol)
//[nk]=nophkel(sl,[tol]) : norm of Hankel operator
[lhs,rhs]=argn(0),
if rhs==1 then tol=1000*%eps,end,
if sl==0 then nk=0,return,end,
lf=spec(sl(2)),
if mini(abs(lf))<=tol then
     error('Imaginary axis poles!'),end,
if maxi(real(lf))<tol then nk=0,return,end,
sl=dtsi(sl);
lc=ctr_gram(sl),lo=obs_gram(sl),
vp=spec(lc*lo),vmax=maxi(real(vp)),
nk=sqrt(vmax)
endfunction
