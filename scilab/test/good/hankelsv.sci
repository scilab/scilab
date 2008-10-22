function [nk,W]=hankelsv(sl,tol)
//!
// Copyright INRIA
sl1=sl(1);
if sl1(1)<>'lss' then error('State-space only :use tf2ss'),end
if sl(7)=='d' then error('Continuous time only'),end
sl(7)='c'
//
[lhs,rhs]=argn(0),
if rhs==1 then tol=1000*%eps,end,
lf=spec(sl(2)),
if mini(abs(lf))<=tol then
     error('Imaginary-axis poles!')
end
if maxi(real(lf)) > tol then warning('unstable !'),end,
[sla,sls,d]=dtsi(sl);
lc=ctr_gram(sls),lo=obs_gram(sls),W=lc*lo;
nk=sort(real(spec(W)));
endfunction
