function [lnum,lden,g]=factors(P,flag)
//Given a polynomial or rational P, returns in list lnum polynomials of 
//degree 1 or two which are the factors of numerators of P.
// and in lden the factors of denominator of P. g is the gain.
// if flag=='c' unstable roots are reflected vs the imaginary axis 
// if flag=='d' unstable roots are reflected vs unit circle 
// Copyright INRIA
[LHS,RHS]=argn(0);
if RHS==1 then flag=[];end
if type(P)==2 then [lnum,lden]=pfactors(P,flag);
  if LHS >=3 then error('invalid LHS');end
  return;end
  if type(P)==16 then 
    if typeof(P)=='state-space' then P=ss2tf(P);end
    [lnum,gn]=pfactors(P('num'),flag);
    [lden,gd]=pfactors(P('den'),flag);g=gn/gd;
    if LHS==1 then
      num=g;
      for k=lnum;num=num.*k;end
      den=1;
      for k=lden;den=den.*k;end
      lnum=syslin(P('dt'),num,den);return
    end
  end
endfunction
